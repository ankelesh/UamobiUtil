#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "ScaningCore/NormalCapturer.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/ItemSearchWidget.h"

class InventoryScaningWidget : public AbstractScaningWidget, abstractNode, captureInterface
{
	Q_OBJECT
protected:
	DocResultsWidget* resultScreen;
	ItemSearchWidget* searchScreen;
	NormalCapturer* capturer;

	bool controlsRequired;
	bool manSelected;
	int controlsAvailable;

public:
	InventoryScaningWidget(GlobalAppSettings& go, QWidget* parent);
	void setDocument(Document);
protected:
	// Inherited via captureInterface
	virtual bool isManualInFocus() override;

	virtual bool handleScannedBarcode() override;

	virtual bool handleNumberInbuffer() override;

	virtual int flushControl(int) override;

	virtual void processBackPress() override;

	virtual void removeManualFocus() override;

	virtual void setControlFocus(int) override;

	virtual bool isControlFocused() override;

	virtual void setDocument(parsedOrder) override;
	virtual void processNumber(QString) override;
	virtual void useControls(QVector<QPair<QString, QString> >&) override;
	virtual void processBarcode(QString) override;

	// Inherited via AbstractScaningWidget
protected slots:
	virtual void submitPressed() override;
	virtual void searchRequired() override;
	virtual void backNeeded() override;
	virtual void switchedFocus() override;
	virtual void syncControlAndBuffer(QString v) override;
	virtual void barcodeConfirmed() override;
	void item_scaned_response();
	void item_confirmed_response();
	void document_confirmed_response();
	virtual void hideCurrent();
	virtual void saveSuccesfull();
	virtual void itemObtained(parsedItemSimplified);
};