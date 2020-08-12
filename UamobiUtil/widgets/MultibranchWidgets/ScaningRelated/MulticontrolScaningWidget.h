#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "widgets/MultibranchWidgets/ScaningRelated/ControlListWidget.h"
#include "widgets/parents/abstractNodeInterface.h"


class MulticontrolScaningWidget : public AbstractScaningWidget, protected abstractNode
{
	Q_OBJECT
protected:
	// subbranches for obtaining items and document results.
	IndependentBranchNode* resultScreen;
	IndependentBranchNode* searchScreen;

	MegaIconButton* toControls;
	ControlListWidget* controlList;

	QueryCache localCache;

	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	explicit MulticontrolScaningWidget(QWidget* parent = Q_NULLPTR, 
		IndependentBranchNode* resScreen = Q_NULLPTR,
		IndependentBranchNode* searchScr = Q_NULLPTR);
	// locks old or creates new document
protected:
	void setDocument(FullDocument);
	void setDocument(Order);
	// creates controls and connects them
    virtual void useControls(const IControlList&) override;
	virtual void wipe() override;
protected slots:
	// sends query to add item using data from controls
	virtual void submitPressed() override;
	// opens search subbranch
	virtual void searchRequired() override;
	// opens document subbranch
	virtual void backNeeded() override;
	// sends query to obtain item data
	virtual void barcodeConfirmed() override;
	// parses response from confirmed query, trying to find richtext and controls
	virtual void item_scaned_response();
	// parses response from confirmed query, then tries to scan again if field is not empty
	virtual void item_confirmed_response();
	// sets up scaning widget with values obtained from received document
	virtual void document_confirmed_response();
	// hides current branch without delete
	virtual void hideCurrent();
	// emits done()
	virtual void saveSuccesfull(RecEntity);
	// inserts code of received entity into barcode field, then calls barcodeConfirmed
	virtual void itemObtained(RecEntity);
	// inserts code into barcode field, then calls barcodeConfirmed
	virtual void barcodeCaught(QString);
	
	void openControlList();
};
