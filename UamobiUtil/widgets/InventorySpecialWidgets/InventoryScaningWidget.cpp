#include "InventoryScaningWidget.h"
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
InventoryScaningWidget::InventoryScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent), abstractNode(), captureInterface(),
	resultScreen(new DocResultsWidget(go, this)), searchScreen(new ItemSearchWidget(go, this)),
	capturer(new NormalCapturer(this, this)), manSelected(false)
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
#ifdef Q_OS_ANDROID
	barcodeField->installEventFilter(new filters::LineEditHelper(this));
#endif
#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &DocResultsWidget::backRequired, this, &InventoryScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::backRequired, this, &InventoryScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::itemSelected, this, &InventoryScaningWidget::itemObtained);
	QObject::connect(resultScreen, &DocResultsWidget::documentSaved, this, &InventoryScaningWidget::saveSuccess);
#else
	QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(itemSelected(parsedItemSimplified)), this, SLOT(itemObtained(parsedItemSimplified)));
	QObject::connect(resultScreen, SIGNAL(documentSaved()), this, SLOT(saveSuccesfull()));
#endif
}

void InventoryScaningWidget::processBarcode(QString bc)
{
	barcodeField->clear();
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void InventoryScaningWidget::submitPressed()
{
	if (!checkControls() || awaiter.isAwaiting())
	{
		return;
	}
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &InventoryScaningWidget::item_confirmed_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	globalSettings.networkingEngine->invSubmit(barcodeField->text(), first_control->getValue(), "", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	first_control->reset();
	barcodeField->clear();
}

void InventoryScaningWidget::barcodeConfirmed()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
	this->setFocus();
#ifdef DEBUG
	detrace_METHEXPL("got text " << barcodeField->text() << " while preparing data");
#endif
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &InventoryScaningWidget::item_scaned_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
	globalSettings.networkingEngine->itemGetInfo(barcodeField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void InventoryScaningWidget::processNumber(QString n)
{
	barcodeField->setText(barcodeField->text() + n);
}

void InventoryScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void InventoryScaningWidget::backNeeded()
{
	_hideAny(resultScreen);
}

void InventoryScaningWidget::useControls(QVector<QPair<QString, QString> >& cvals)
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

void InventoryScaningWidget::switchedFocus()
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

void InventoryScaningWidget::syncControlAndBuffer(QString v)
{
	numberBuffer = v;
}

void InventoryScaningWidget::item_scaned_response()
{
	parse_uniresults_functions::CombinedNamedLinearResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	mainTextView->setText(resp.named.value("richdata"));
	itemSuppliedValues = resp.named;
	useControls(resp.linear);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void InventoryScaningWidget::item_confirmed_response()
{
	parse_uniresults_functions::CombinedNamedLinearResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	itemSuppliedValues = resp.named;
	mainTextView->setText(itemSuppliedValues.value("richdata"));
	useControls(resp.linear);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
	if (!barcodeField->text().isEmpty())
	{
		awaiter.stopAwaiting();
		barcodeConfirmed();
	}
}

void InventoryScaningWidget::document_confirmed_response()
{
	document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
	userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);

#ifdef Q_OS_WINCE
    userInfo->setText(normalizeLine(modename  + " (" + document.docId + ")\n" + document.supplier));
#else
    userInfo->setText(modename  + " (" + document.docId + ")\n" + document.supplier);
#endif
	hideProcessingOverlay();
}

void InventoryScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void InventoryScaningWidget::saveSuccesfull()
{
    _hideCurrent(innerWidget);
	emit saveSuccess();
}

void InventoryScaningWidget::itemObtained(parsedItemSimplified item)
{
	barcodeField->setText(item.barcode);
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void InventoryScaningWidget::_postClear()
{
	resultScreen->clear();
	searchScreen->clear();
	_hideCurrent(innerWidget);
}

void InventoryScaningWidget::setDocument(parsedOrder)
{
}

void InventoryScaningWidget::setDocument(Document doc)
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &InventoryScaningWidget::document_confirmed_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
	globalSettings.networkingEngine->invNew(QDate::currentDate(), doc.docId, doc.comment, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

bool InventoryScaningWidget::isManualInFocus()
{
	if (controlsAvailable == 1)
	{
		return first_control->hasFocus();
	}
	return false;
}

bool InventoryScaningWidget::handleScannedBarcode()
{
	if (!barcodeBuffer.isEmpty()) {
		processBarcode(barcodeBuffer);

		return true;
	}
	return false;
}

bool InventoryScaningWidget::handleNumberInbuffer()
{
	refreshControls();
	return true;
}

int InventoryScaningWidget::flushControl(int cnum)
{
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

void InventoryScaningWidget::processBackPress()
{
	emit backRequired();
}

void InventoryScaningWidget::removeManualFocus()
{
	this->setFocus();
	manSelected = false;
}

void InventoryScaningWidget::setControlFocus(int)
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

bool InventoryScaningWidget::isControlFocused()
{
	return AbstractScaningWidget::isControlFocused();
}
