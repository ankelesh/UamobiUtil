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
	virtual void useControls(QVector<QPair<QString, QString> >&) override;
	virtual bool isManualInFocus() override;

	virtual bool handleScannedBarcode() override;

	virtual bool handleNumberInbuffer() override;

	virtual void processBackPress() override;

	virtual void removeManualFocus() override;

	virtual void setControlFocus(int) override;

	virtual int flushControl(int) override;
	virtual bool isControlFocused()override;

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
	virtual void switchedFocus() override;
	virtual void syncControlAndBuffer(QString v) override;

	// Inherited via captureInterface

	// Inherited via AbstractScaningWidget

	// Inherited via AbstractScaningWidget
};
