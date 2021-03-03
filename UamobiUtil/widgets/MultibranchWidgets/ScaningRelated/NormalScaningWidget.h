#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"
#include "datacore/EntityListEntity.h"

/*
	This widget provides "normal" scaning window which corresponds to most modes:
	two slots for controls, result screen and search screen. It has two states
	of flow - awaiting barcode and awaiting control fill. When awaiting barcode,
	no controls are shown (exception if server gives extra controls).
	When awaiting control fill, both (if exists) controls are shown and connected
	and confirm button is working. This widget can be primed with document or order entity
	to lock document or open new one.
*/


using QueryTemplates::QueryCache;
class NormalScaningWidget : public AbstractScaningWidget, abstractNode
{
	Q_OBJECT
protected:
	// subbranches for obtaining items and document results.
	IndependentBranchNode* resultScreen;
	IndependentBranchNode* searchScreen;

	QueryCache localCache;
	ERecordList extraParentDocuments;
	// upcasts record and calls corresponding setter
	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	explicit NormalScaningWidget(QWidget* parent = Q_NULLPTR, IndependentBranchNode* resScreen = Q_NULLPTR,
		IndependentBranchNode* searchScr = Q_NULLPTR);
	// locks old or creates new document
protected:
	void setDocument(FullDocument);
	void setDocument(Order);
	virtual void setDocument(RecEntity) override;
	// creates controls and connects them
    virtual void useControls(const IControlList &) override;
	virtual void registerExtraParents(QString docId);

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
	virtual void docs_registration_complete();
};
