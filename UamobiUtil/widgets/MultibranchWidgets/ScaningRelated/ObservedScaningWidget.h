#pragma once
#include "MulticontrolScaningWidget.h"




class ObservedScaningWidget : public MulticontrolScaningWidget
{
	Q_OBJECT
protected:
	IndependentBranchNode* observerNode;
	RecEntity itemFromObserver;
	
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	// sends query to add item using data from controls
	virtual void barcodeConfirmed() override;
	virtual void item_confirmed_response() override;
public:
	ObservedScaningWidget(QWidget* parent = Q_NULLPTR,
		IndependentBranchNode* observer = Q_NULLPTR,
		IndependentBranchNode* resScreen = Q_NULLPTR,
		IndependentBranchNode* searchScr = Q_NULLPTR
	);
protected slots:
	void observerDone(RecEntity e);

};