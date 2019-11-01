#include "InventoryScaningWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
InventoryScaningWidget::InventoryScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent), abstractNode(), captureInterface(),
	resultScreen(new DocResultsWidget(go, this)), searchScreen(new ItemSearchWidget(go, this)),
	capturer(new NormalCapturer(this, this)), qtyRequired(false), manSelected(false)
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
	if (!qtyRequired || awaiter.isAwaiting())
	{
		return;
	}
	if (!controlsList.contains("qty"))
	{
		return;
	}
	if (!controlsList.value("qty")->canGiveValue())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &InventoryScaningWidget::item_confirmed_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	globalSettings.networkingEngine->invSubmit(barcodeField->text(), controlsList.value("qty")->getValue(),"", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
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
	stateInfo->setText(tr("receipt_scaning_quering"));
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

void InventoryScaningWidget::useControls()
{
	numberBuffer.clear();
#ifdef DEBUG
	detrace_METHCALL("useControls");
	detrace_METHEXPL("is containing qty: " << itemSuppliedValues.contains("qty"));
#endif
	if (itemSuppliedValues.contains("qty"))
	{
		if (!controlsList.contains("qty")) {
			controlsList.insert("qty", new QuantityControl(innerWidget));
			innerLayout->insertWidget(innerLayout->count() - 1, controlsList.value("qty"));
			controlsList.value("qty")->installEventFilter(keyfilter);
			QObject::connect(controlsList.value("qty"), &abs_control::valueChanged, this, &InventoryScaningWidget::syncControlAndBuffer);
		}
		controlsList.value("qty")->setAwaiting();
		capturer->setControlNumber(1);
		qtyRequired = true;
		submitButton->setDisabled(false);
		this->setFocus();
	}
	else
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->reset();
			controlsList.value("qty")->hide();
		}
		setFocus();
		qtyRequired = false;
		submitButton->setDisabled(true);
	}
}

void InventoryScaningWidget::switchedFocus()
{
	if (qtyRequired)
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->setFocus();
		}
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
	stateInfo->setText(tr("Success!"));
	parse_uniresults_functions::PositionalResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	mainTextView->setText(resp.values.value("richdata"));
	itemSuppliedValues = resp.values;
	useControls();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void InventoryScaningWidget::item_confirmed_response()
{
	itemSuppliedValues = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext).values;
	mainTextView->setText(itemSuppliedValues.value("richdata"));
	useControls();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
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
	if (controlsList.contains("qty"))
	{
		return controlsList.value("qty")->hasFocus();
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
	if (qtyRequired)
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->setValue(numberBuffer);
			return true;
		}
		//detrace_METHEXPL("There was no qty controller");
	}
	return false;
}

int InventoryScaningWidget::flushControl(int)
{
	return 0;
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
	if (qtyRequired)
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->setFocus();
			manSelected = false;
		}
	}
}

bool InventoryScaningWidget::isControlFocused()
{
	if (controlsList.contains("qty"))
	{
		return controlsList.value("qty")->hasFocus();
	}
	return false;
}
