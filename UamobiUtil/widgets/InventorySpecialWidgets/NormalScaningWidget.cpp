#include "NormalScaningWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"


void NormalScaningWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
		return;
	if (e->myType() == UniformXmlObject::Order)
	{
		setDocument(upcastRecord<OrderEntity>(e));
	}
	else if (e->myType() == UniformXmlObject::Document)
	{
		setDocument(upcastRecord<FullDocumentEntity>(e));
	}
}

NormalScaningWidget::NormalScaningWidget(QWidget* parent,
		QueryTemplates::OverloadableQuery gII,
		QueryTemplates::OverloadableQuery aI,
		QueryTemplates::OverloadableQuery nD)
	: AbstractScaningWidget(independent_nodes::NormalScaning, parent), 
	abstractNode(), captureInterface(),
	resultScreen(new DocResultsWidget(this)), 
	searchScreen(new PagedSearchWidget(RecEntity(new ShortItemEntity()), this)),
	capturer(new NormalCapturer(this, this)),
	getItemInfo(gII), addItem(aI), newDocument(nD),
	manSelected(false)
{
	mainLayout->addWidget(searchScreen);
	mainLayout->addWidget(resultScreen);
	searchScreen->hide();
	resultScreen->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	submitButton->setDisabled(true);
	mainTextView->installEventFilter(capturer->keyfilter);
	QScroller::grabGesture(mainTextView, QScroller::LeftMouseButtonGesture);
	innerWidget->installEventFilter(capturer->keyfilter);

	if (getItemInfo.isDefault())
	{
		getItemInfo = QueryTemplates::OverloadableQuery(QueryTemplates::getItemInfo,
			QStringList{ "barcode", "printer" }, QStringList{ "barcode", "printer" });
	}
	if (addItem.isDefault())
	{
		addItem = QueryTemplates::OverloadableQuery(QueryTemplates::receiptAddItem,
			QStringList{ "barcode", "control1", "control2", "show_code", "params" },
			QStringList{ "barcode", "control1" });
	}
	if (newDocument.isDefault())
	{
		newDocument = QueryTemplates::OverloadableQuery(QueryTemplates::receiptNewDocument,
			QStringList{ "date", "parent", "comment" },
			QStringList{ "date", "parent", "comment" });
	}


#ifdef Q_OS_ANDROID
	barcodeField->installEventFilter(new filters::LineEditHelper(this));
#endif
#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &DocResultsWidget::backRequired, this, &NormalScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &PagedSearchWidget::backRequired, this, &NormalScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &PagedSearchWidget::done, this, &NormalScaningWidget::itemObtained);
	QObject::connect(resultScreen, &DocResultsWidget::documentSaved, this, &NormalScaningWidget::saveSuccess);
#else
	QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(itemSelected(parsedItemSimplified)), this, SLOT(itemObtained(parsedItemSimplified)));
	QObject::connect(resultScreen, SIGNAL(documentSaved()), this, SLOT(saveSuccesfull()));
#endif
}

void NormalScaningWidget::processBarcode(QString bc)
{
	barcodeField->clear();
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void NormalScaningWidget::submitPressed()
{
	if (!checkControls() || awaiter->isAwaiting())
	{
		return;
	}
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::item_confirmed_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	QStringList buffer;
	buffer << barcodeField->text();
	buffer << ((first_control.isNull()) ? "" : first_control->getValue());
	buffer << ((second_control.isNull()) ? "" : second_control->getValue());
	buffer << "";
	buffer << "";
	AppNetwork->execQueryByTemplate(addItem, 5, buffer, awaiter);
	first_control->reset();
	barcodeField->clear();
}

void NormalScaningWidget::barcodeConfirmed()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
	this->setFocus();
#ifdef DEBUG
	detrace_METHEXPL("got text " << barcodeField->text() << " while preparing data");
#endif
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::item_scaned_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
	AppNetwork->execQueryByTemplate(getItemInfo, barcodeField->text(), "", awaiter);
}

void NormalScaningWidget::processNumber(QString n)
{
	barcodeField->setText(barcodeField->text() + n);
}

void NormalScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void NormalScaningWidget::backNeeded()
{
	_hideAny(resultScreen);
}

void NormalScaningWidget::useControls(IControlList & cvals)
{
	AbstractScaningWidget::useControls(cvals);
	switch (controlsAvailable)
	{
	case 1:
	case 2:
		first_control->associateBuffer(numberBuffer);
	case 0:
		capturer->setControlNumber(controlsAvailable);
	}
}

void NormalScaningWidget::switchedFocus()
{
	if (controlsRequired)
	{
		if (controlsAvailable >= 1)
		{
			focusControl(0);
		}
		manSelected = !manSelected;
	}
	else
	{
		if (manSelected)
		{
			this->setFocus();
		}
		else
		{
			barcodeField->setFocus();
		}
		manSelected = !manSelected;
	}
}

void NormalScaningWidget::syncControlAndBuffer(QString v)
{
	numberBuffer = v;
}

void NormalScaningWidget::item_scaned_response()
{
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response = 
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (response.isError || response.additionalObjects.isEmpty())
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		mainTextView->setText(response.additionalObjects.first()->value("richdata"));
		itemSuppliedValues.clear();
		itemSuppliedValues = response.additionalObjects.first()->directFieldsAccess();
	}
	useControls(response.objects);
	capturer->setPhase(0);
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void NormalScaningWidget::item_confirmed_response()
{
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response =
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (response.isError || response.additionalObjects.isEmpty())
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		mainTextView->setText(response.additionalObjects.first()->value("richdata"));
		itemSuppliedValues.clear();
		itemSuppliedValues = response.additionalObjects.first()->directFieldsAccess();
	}
	useControls(response.objects);
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
	if (!barcodeField->text().isEmpty())
	{
		awaiter->stopAwaiting();
		barcodeConfirmed();
	}
}

void NormalScaningWidget::document_confirmed_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<FullDocumentEntity> response =
		RequestParser::parseResponse<FullDocumentEntity>(parser);
	if (response.isError)
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		if (response.isEmpty())
		{
			userInfo->setText(tr("document creation error"));
		}
		else
		{
			document = response.objects.first();
			userInfo->setText(tr("receipt_scaning_mode_name") + 
				"(" + document->docId + ")\n" + document->supplier);
#ifdef Q_OS_WINCE
			userInfo->setText(normalizeLine(modename + " (" + document.docId + ")\n" + document.supplier));
#else
			userInfo->setText(modename + " (" + document->docId + ")\n" + document->supplier);
#endif
		}
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void NormalScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void NormalScaningWidget::saveSuccesfull()
{
    _hideCurrent(innerWidget);
	emit saveSuccess();
}

void NormalScaningWidget::itemObtained(RecEntity item)
{
	ShortItem temp;
	temp = upcastRecord(item, temp);
	if (temp.isNull())
		return;
	barcodeField->setText(temp->code);
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void NormalScaningWidget::_sendDataRequest()
{
}

void NormalScaningWidget::_postClear()
{
	resultScreen->clear();
	searchScreen->clear();
	_hideCurrent(innerWidget);
}

void NormalScaningWidget::setDocument(Order o)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::document_confirmed_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
	AppNetwork->execQueryByTemplate(newDocument,
		QDate::currentDate().toString(), o->code, "", awaiter);
}

void NormalScaningWidget::setDocument(FullDocument doc)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::document_confirmed_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
	AppNetwork->execQueryByTemplate(newDocument, 
		QDate::currentDate().toString(), document->docId, document->comment, awaiter);
	isFiltersRequired = true;
}

bool NormalScaningWidget::isManualInFocus()
{
	if (controlsAvailable == 1)
	{
		return first_control->hasFocus();
	}
	return false;
}

bool NormalScaningWidget::handleScannedBarcode()
{
	if (!barcodeBuffer.isEmpty()) {
		processBarcode(barcodeBuffer);

		return true;
	}
	return false;
}

bool NormalScaningWidget::handleNumberInbuffer()
{
	refreshControls();
	return true;
}

int NormalScaningWidget::flushControl(int cnum)
{
	if (controlsAvailable == 0)
		return 0;
	if (controlsAvailable == 1)
		submitPressed();
	else
	{
		switch (cnum)
		{
		case 0:
			first_control->unassociateBuffer();
			second_control->associateBuffer(numberBuffer);
			numberBuffer.clear();
			second_control->setAwaiting();
			break;
		case 1:
			submitPressed();
		}
	}
	return cnum + 1;
}

void NormalScaningWidget::processBackPress()
{
	emit backRequired();
}

void NormalScaningWidget::removeManualFocus()
{
	this->setFocus();
	manSelected = false;
}

void NormalScaningWidget::setControlFocus(int)
{
	if (controlsRequired)
	{
		if (controlsAvailable == 1)
		{
			first_control->setFocus();
			manSelected = false;
		}
	}
}

bool NormalScaningWidget::isControlFocused()
{
	return AbstractScaningWidget::isControlFocused();
}
