#pragma once
#include "inframedWidget.h"
#include "networking/things.h"



namespace independent_nodes
{
	enum nodelist
	{
		PagedSearch,
		FilterSelect,
		DocResults,
		ReceiptParameters,
		NormalScaning,
		IdDependentSelect,
		SelectItemFromList,
		PlaceSelect,
		InventoryParameters,
		ParentDocument,
		BarcodeFilterSelect,
		Subbranch
	};

	
}


class IndependentBranchNode : public inframedWidget
{
	Q_OBJECT
protected:
	int id;
	int backtracksTo;

	
	virtual void _handleRecord(RecEntity);
	virtual void _sendDataRequest() = 0;
public:
	IndependentBranchNode(int Id, QWidget* parent = Q_NULLPTR) 
		: inframedWidget(parent), id(Id) {};
	IndependentBranchNode(int Id, bool iFilter, QWidget* parent = Q_NULLPTR) 
		: inframedWidget(iFilter, parent), id(Id) {};
	int getId() { return id; };
	int& backtracking() {	return backtracksTo;	};
	void processRecord(RecEntity);
	void loadData();
signals:
	void done(RecEntity);
};