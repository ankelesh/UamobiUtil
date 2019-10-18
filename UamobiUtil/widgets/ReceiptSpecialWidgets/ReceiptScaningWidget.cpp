#include "ReceiptScaningWidget.h"
#include "debugtrace.h"
#include <QApplication>

ReceiptScaningWidget::ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent),abstractNode(), captureInterface(), resultScreen(new DocResultsWidget(go,this)),
	searchScreen(new ItemSearchWidget(go,this)), capturer(new NormalCapturer(this, this))
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
    innerWidget->installEventFilter(capturer->keyfilter);
    barcodeField->installEventFilter(new filters::LineEditHelper(this));
#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &DocResultsWidget::backRequired, this, &ReceiptScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::backRequired, this, &ReceiptScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::itemSelected, this, &ReceiptScaningWidget::itemObtained);
	QObject::connect(resultScreen, &DocResultsWidget::documentSaved, this, &ReceiptScaningWidget::saveSuccesfull);
#else
    QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(searchScreen, SIGNAL(itemSelected(parsedItemSimplified)), this, SLOT(itemObtained(parsedItemSimplified)));
    QObject::connect(resultScreen, SIGNAL(documentSaved()), this, SLOT(saveSuccesfull()));
#endif
}


void ReceiptScaningWidget::submitPressed()
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
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_confirmed_response);
	globalSettings.networkingEngine->recSubmit(barcodeField->text(), controlsList.value("qty")->getValue(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void ReceiptScaningWidget::processNumber(QString num)
{
	barcodeField->setText(barcodeField->text() + num);
}

void ReceiptScaningWidget::processBarcode(QString bc)
{
    //detrace_METHEXPL("processing barcode as " << bc << " while replacing " << barcodeField->text());
    barcodeField->clear();
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void ReceiptScaningWidget::barcodeConfirmed()
{
	if (awaiter.isAwaiting())
		return;
    this->setFocus();
    //detrace_METHEXPL("got text " << barcodeField->text() << " while preparing data");
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_scaned_response);
	globalSettings.networkingEngine->itemGetInfo(barcodeField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	stateInfo->setText(tr("receipt_scaning_quering"));
}

void ReceiptScaningWidget::setDocument(parsedOrder po)
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::document_confirmed_response);
	globalSettings.networkingEngine->recNew(QDate::currentDate(), po.code, "", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	//globalSettings.networkingEngine->docLock(document.docId, &awaiter, RECEIVER_SLOT_NAME);
}

void ReceiptScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void ReceiptScaningWidget::itemObtained(parsedItemSimplified item)
{
	barcodeField->setText(item.barcode);
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void ReceiptScaningWidget::saveSuccesfull()
{
	emit backRequired();
}

bool ReceiptScaningWidget::isManualInFocus()
{
	return barcodeField->hasFocus();
}

bool ReceiptScaningWidget::handleScannedBarcode()
{
	if (!barcodeBuffer.isEmpty()) {
		processBarcode(barcodeBuffer);

		return true;
	}
	return false;
}

bool ReceiptScaningWidget::handleNumberInbuffer()
{
	if (qtyRequired)
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->setValue(numberBuffer);
			return true;
		}
	}
	return false;
}

void ReceiptScaningWidget::processBackPress()
{
    emit backRequired();
}

void ReceiptScaningWidget::removeManualFocus()
{
    this->setFocus();
}

void ReceiptScaningWidget::setControlFocus(int val)
{
	if (qtyRequired)
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->setFocus();
		}
	}
}


void ReceiptScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void ReceiptScaningWidget::backNeeded()
{
	_hideAny(resultScreen);
}

void ReceiptScaningWidget::useControls()
{

#ifdef DEBUG
	detrace_METHCALL("useControls");
	detrace_METHEXPL("is containing qty: " << itemSuppliedValues.contains("qty"));
#endif
	if (itemSuppliedValues.contains("qty"))
	{
		if (!controlsList.contains("qty")) {
			controlsList.insert("qty", new QuantityControl(innerWidget));
			innerLayout->insertWidget(innerLayout->count() - 1, controlsList.value("qty"));
		}
		controlsList.value("qty")->setAwaiting();
		qtyRequired = true;
		submitButton->setDisabled(false);
	}
	else
	{
		if (controlsList.contains("qty"))
		{
			controlsList.value("qty")->reset();
			controlsList.value("qty")->hide();
		}
		qtyRequired = false;
		submitButton->setDisabled(true);
	}
}

void ReceiptScaningWidget::item_scaned_response()
{
	stateInfo->setText(tr("Success!"));
	parse_uniresults_functions::PositionalResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	mainTextView->setText(resp.values.value("richdata"));
	itemSuppliedValues = resp.values;
	useControls();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
}

void ReceiptScaningWidget::item_confirmed_response()
{
	itemSuppliedValues = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext).values;
	mainTextView->setText(itemSuppliedValues.value("richdata"));
	useControls();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(item_confirmed_response));
}

void ReceiptScaningWidget::document_confirmed_response()
{
	document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
	userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	awaiter.disconnect(SIGNAL(requestReceived), this, SLOT(document_confirmed_response()));
}

void ReceiptScaningWidget::was_timeout()
{
	AbstractScaningWidget::was_timeout();
	awaiter.disconnect(SIGNAL(requestReceived), this, SLOT(document_confirmed_response()));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(item_confirmed_response));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
}
