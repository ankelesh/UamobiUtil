#include "ReceiptScaningWidget.h"

ReceiptScaningWidget::ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent)
{
}

void ReceiptScaningWidget::photoPressed()
{
}

void ReceiptScaningWidget::submitPressed()
{
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
		parse_uniresults_functions::PairedResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
		mainTextView->setText(resp.primaryResult);
		
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
}
