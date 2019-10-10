#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/ItemSearchWidget.h"
#include "widgets/parents/AbstractScaningWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "networking/RequestAwaiter.h"

class ReceiptScaningWidget : public AbstractScaningWidget, abstractNode
{
	Q_OBJECT
protected:
	
	DocResultsWidget* resultScreen;
	ItemSearchWidget* searchScreen;


	bool qtyRequired;
public:
	ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent);

protected:
	virtual void submitPressed() override;
	virtual void processNumber(QString) override;
	virtual void processBarcode(QString) override;
	virtual void barcodeConfirmed() override;
	virtual void searchRequired() override;
	virtual void backNeeded() override;
	virtual void useControls() override;
public slots:
	virtual void setDocument(parsedOrder);
	virtual void hideCurrent();
	virtual void itemObtained(parsedItemSimplified);
	virtual void saveSuccesfull();

};
