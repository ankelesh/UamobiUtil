#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/ItemSearchWidget.h"
#include "widgets/parents/AbstractScaningWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "networking/RequestAwaiter.h"
#include "ScaningCore/NormalCapturer.h"

class ReceiptScaningWidget : public AbstractScaningWidget, abstractNode, captureInterface
{
	Q_OBJECT
protected:
	
	DocResultsWidget* resultScreen;
	ItemSearchWidget* searchScreen;
	NormalCapturer* capturer;

	bool qtyRequired;
	bool manSelected;
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

protected slots:
	void item_scaned_response();
	void item_confirmed_response();
	void document_confirmed_response();
	virtual void was_timeout() override;
public slots:
	virtual void setDocument(parsedOrder);
	virtual void hideCurrent();
	virtual void itemObtained(parsedItemSimplified);
	virtual void saveSuccesfull();




	// Inherited via captureInterface
	virtual bool isManualInFocus() override;

	virtual bool handleScannedBarcode() override;

	virtual bool handleNumberInbuffer() override;

	virtual void processBackPress() override;

    virtual void removeManualFocus() override;

	virtual void setControlFocus(int) override;

	virtual int flushControl(int) override;

	// Inherited via AbstractScaningWidget
	virtual void switchedFocus() override;

	// Inherited via AbstractScaningWidget
	virtual void syncControlAndBuffer(QString v) override;
	virtual bool isControlFocused()override;
};
