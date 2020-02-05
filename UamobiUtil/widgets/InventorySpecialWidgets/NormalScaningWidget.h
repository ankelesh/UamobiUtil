#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "ScaningCore/NormalCapturer.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"



class NormalScaningWidget : public AbstractScaningWidget, abstractNode, captureInterface
{
	Q_OBJECT
protected:
	DocResultsWidget* resultScreen;
	PagedSearchWidget* searchScreen;
	NormalCapturer* capturer;

	QueryTemplates::OverloadableQuery getItemInfo;
	QueryTemplates::OverloadableQuery addItem;
	QueryTemplates::OverloadableQuery newDocument;

	bool manSelected;
	bool isFiltersRequired;
	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	NormalScaningWidget(QWidget* parent = Q_NULLPTR,
		QueryTemplates::OverloadableQuery gII = QueryTemplates::OverloadableQuery::defaultQuery(),
		QueryTemplates::OverloadableQuery aI = QueryTemplates::OverloadableQuery::defaultQuery(),
		QueryTemplates::OverloadableQuery nD = QueryTemplates::OverloadableQuery::defaultQuery());
	void setDocument(FullDocument);
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

	virtual void setDocument(Order) override;
	virtual void processNumber(QString) override;
	virtual void useControls(IControlList &) override;
	virtual void processBarcode(QString) override;
	// Inherited via AbstractScaningWidget
	virtual void _postClear() override;

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
	virtual void itemObtained(RecEntity);

signals:
	void filterRequired(QString resp);

	// Inherited via AbstractScaningWidget
};