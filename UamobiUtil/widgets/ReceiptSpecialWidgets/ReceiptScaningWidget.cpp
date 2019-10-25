#include "ReceiptScaningWidget.h"
#include "debugtrace.h"
#include <QApplication>
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/ElementWidgets/ProcessingOverlay.h"

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
	QScroller::grabGesture(mainTextView, QScroller::LeftMouseButtonGesture);
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
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_confirmed_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
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
	showProcessingOverlay();
    this->setFocus();

#ifdef DEBUG
	detrace_METHEXPL("got text " << barcodeField->text() << " while preparing data");
#endif
#ifdef QT_VERSION5X
    QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_scaned_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
    globalSettings.networkingEngine->itemGetInfo(barcodeField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	stateInfo->setText(tr("receipt_scaning_quering"));
}

void ReceiptScaningWidget::setDocument(parsedOrder po)
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::document_confirmed_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
    globalSettings.networkingEngine->recNew(QDate::currentDate(), po.code, "", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	
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

#ifdef DEBUG
	//detrace_METHCALL("handleNumberInBuffer");
#endif

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

int ReceiptScaningWidget::flushControl(int)
{
	submitPressed();
	return 1;
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

void ReceiptScaningWidget::item_scaned_response()
{
	stateInfo->setText(tr("Success!"));
	parse_uniresults_functions::PositionalResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	mainTextView->setText(resp.values.value("richdata"));
	itemSuppliedValues = resp.values;
	useControls();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ReceiptScaningWidget::item_confirmed_response()
{
	itemSuppliedValues = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext).values;
	mainTextView->setText(itemSuppliedValues.value("richdata"));
	useControls();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ReceiptScaningWidget::document_confirmed_response()
{
	document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
	userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ReceiptScaningWidget::was_timeout()
{
	AbstractScaningWidget::was_timeout();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}
