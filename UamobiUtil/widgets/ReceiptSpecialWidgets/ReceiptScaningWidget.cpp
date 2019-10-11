#include "ReceiptScaningWidget.h"

ReceiptScaningWidget::ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent),abstractNode(), resultScreen(new DocResultsWidget(go,this)),
	searchScreen(new ItemSearchWidget(go,this))
{
	mainLayout->addWidget(searchScreen);
	mainLayout->addWidget(resultScreen);
	searchScreen->hide();
	resultScreen->hide();
	current = innerWidget;
	untouchable = innerWidget;

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
	if (!qtyRequired)
	{
		return;
	}
	if (quantitySpin->value() == 0)
	{
		return;
	}
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->recSubmit(barcodeField->text(), QString::number(quantitySpin->value()), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		mainTextView->setText(tr("scaning_widget_timeout"));
	}
	else
	{
		itemSuppliedValues = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext).values;
		mainTextView->setText(itemSuppliedValues.value("richdata"));
		useControls();
	}
}

void ReceiptScaningWidget::processNumber(QString num)
{
	barcodeField->setText(barcodeField->text() + num);
}

void ReceiptScaningWidget::processBarcode(QString bc)
{
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void ReceiptScaningWidget::barcodeConfirmed()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->itemGetInfo(barcodeField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	stateInfo->setText(tr("receipt_scaning_quering"));
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		mainTextView->setText("receipt_scaning_timeout");
	}
	else
	{
		parse_uniresults_functions::PositionalResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
		mainTextView->setText(resp.values.value("richdata"));
		itemSuppliedValues = resp.values;
		useControls();
	}
}

void ReceiptScaningWidget::setDocument(parsedOrder po)
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->recNew(QDate::currentDate(), po.code, "", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		emit backRequired();
	}
	else
	{
		document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
		userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	}
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
		quantitySpin->show();
		qtyRequired = true;
	}
	else
	{
		quantitySpin->hide();
		qtyRequired = false;
	}
}
