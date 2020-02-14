#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"


using QueryTemplates::QueryCache;
class NormalScaningWidget : public AbstractScaningWidget, abstractNode
{
	Q_OBJECT
protected:
	IndependentBranchNode* resultScreen;
	IndependentBranchNode* searchScreen;

	QueryCache localCache;


	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	explicit NormalScaningWidget(QWidget* parent = Q_NULLPTR, IndependentBranchNode* resScreen = Q_NULLPTR,
		IndependentBranchNode* searchScr = Q_NULLPTR);
	void setDocument(FullDocument);
protected:
	virtual void setDocument(Order) override;
	virtual void useControls(IControlList &) override;

protected slots:
	virtual void submitPressed() override;
	virtual void searchRequired() override;
	virtual void backNeeded() override;
	virtual void barcodeConfirmed() override;
	virtual void item_scaned_response();
	virtual void item_confirmed_response();
	virtual void document_confirmed_response();
	virtual void hideCurrent();
	virtual void saveSuccesfull(RecEntity);
	virtual void itemObtained(RecEntity);
	virtual void barcodeCaught(QString);
};