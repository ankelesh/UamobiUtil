#pragma once

#include "widgets/parents/AbstractScaningWidget.h"
#include "networking/RequestAwaiter.h"


class ReceiptScaningWidget : public AbstractScaningWidget
{
protected:
	
public:
	ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent);


protected slots:
	// Inherited via AbstractScaningWidget
	virtual void photoPressed() override;

	virtual void submitPressed() override;

	virtual void processNumber(QString) override;

	virtual void processBarcode(QString) override;

	virtual void barcodeConfirmed() override;
public slots:
	virtual void setDocument(parsedOrder);

};

