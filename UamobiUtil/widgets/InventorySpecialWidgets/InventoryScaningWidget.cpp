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

InventoryScaningWidget::InventoryScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent), abstractNode(), captureInterface(),
	resultScreen(new DocResultsWidget(go, this)), searchScreen(new ItemSearchWidget(go, this)),
	capturer(new NormalCapturer(this, this)), controlsRequired(false), manSelected(false), controlsAvailable(0)
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
	barcodeField->installEventFilter(new filters::LineEditHelper(this));
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
	if (!controlsRequired || awaiter.isAwaiting())
	{
		return;
	}
	if (controlsAvailable < 1)
	{
		return;
	}
	if (!first_control->canGiveValue())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &InventoryScaningWidget::item_confirmed_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	globalSettings.networkingEngine->invSubmit(barcodeField->text(), first_control->getValue(),"", &awaiter, RECEIVER_SLOT_NAME);
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

void InventoryScaningWidget::useControls(QVector<QPair<QString, QString> > & cvals)
{
	numberBuffer.clear();
	switch (cvals.length())
	{
	case 0:
		switch (controlsAvailable)
		{
		case 2:
			delete second_control;
			second_control = Q_NULLPTR;
		case 1:
			delete first_control;
			first_control = Q_NULLPTR;
		case 0:
			setFocus();
			controlsRequired = false;
			submitButton->setDisabled(true);
			controlsAvailable = 0;
			controlsRequired = false;
		}
		return;
	case 1:
		if (controlsAvailable >= 1)
		{
			if (first_control->name == cvals.at(0).first)
			{
				first_control->reset();
				first_control->show();
			}
			else
			{
				delete first_control;
				first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
				first_control->installEventFilter(keyfilter);
			}
			if (second_control != Q_NULLPTR)
			{
				delete second_control;
				second_control = Q_NULLPTR;
				--controlsAvailable;
			}
		}
		else
		{
			first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
			first_control->installEventFilter(keyfilter);
			++controlsAvailable;
		}
		capturer->setControlNumber(1);
		first_control->setAwaiting();
		controlsRequired = true;
		submitButton->setDisabled(false);
		this->setFocus();
		return;
	case 2:
	default:
		switch (controlsAvailable)
		{
		case 2:
			if (!(cvals.at(1).first == second_control->name))
			{
				delete second_control;
				second_control = Q_NULLPTR;
			}
		case 1:
			if (!(cvals.at(1).first == first_control->name))
			{
				delete first_control;
				first_control = Q_NULLPTR;
			}
		case 0:
			if (first_control == Q_NULLPTR)
			{

				first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
				first_control->installEventFilter(keyfilter);
			}
			else
			{
				first_control->reset();
			}
			if (second_control == Q_NULLPTR)
			{
				second_control = fabricateControl(cvals.at(0).first, innerLayout, innerWidget);
				second_control->installEventFilter(keyfilter);
			}
			else
			{
				second_control->reset();
			}
		}
		capturer->setControlNumber(2);
		controlsAvailable = 2;
	}
	first_control->setAwaiting();
	controlsRequired = true;
	submitButton->setDisabled(false);
	this->setFocus();
}

void InventoryScaningWidget::switchedFocus()
{
	if (controlsRequired)
	{
		if (controlsAvailable == 1)
		{
			first_control->setFocus();
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
		barcodeConfirmed();
	}
}

void InventoryScaningWidget::document_confirmed_response()
{
	document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
	userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void InventoryScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void InventoryScaningWidget::saveSuccesfull()
{
	emit saveSuccess();
}

void InventoryScaningWidget::itemObtained(parsedItemSimplified item)
{
	barcodeField->setText(item.barcode);
	barcodeConfirmed();
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
	globalSettings.networkingEngine->recNew(QDate::currentDate(), doc.docId, doc.comment, &awaiter, RECEIVER_SLOT_NAME);
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
#ifdef DEBUG
	detrace_METHCALL("inventoryScaningWidget |" << numberBuffer);
#endif
	if (controlsRequired)
	{
		if (controlsAvailable == 1)
		{
			first_control->refresh();
			return true;
		}
		//detrace_METHEXPL("There was no qty controller");
	}
	return false;
}

int InventoryScaningWidget::flushControl(int)
{
	submitPressed();
	return 1;
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
	if (controlsAvailable == 1)
	{
		return first_control->hasFocus();
	}
	return false;
}
