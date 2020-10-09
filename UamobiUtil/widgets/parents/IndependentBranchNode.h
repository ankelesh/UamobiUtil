#pragma once
#include "inframedWidget.h"
#include "networking/things.h"


namespace independent_nodes
{
	enum nodelist
	{
		NotANode,
		PagedSearch,
		FilterSelect,
		DocResults,
		EditableDocResults,
		ReceiptParameters,
		NormalScaning,
		PrintingScaning,
		MulticontrolScaning,
		IdDependentSelect,
		SelectItemFromList,
		PlaceSelect,
		InventoryParameters,
		ParentDocument,
		BarcodeFilterSelect,
		Subbranch,
		StaticSubbranch,
		Switch,
		Sender,
		ObservedScaning,
		ListPickObs,
		SkipNode,
		SwitchByScanBc,
		DefaultNode
	};
}

class BranchException;
class IndependentBranchNode : public inframedWidget
{
	Q_OBJECT	
protected:
	int id;
	int backtracksTo;
	
	virtual void _handleRecord(RecEntity) = 0;
	virtual void _sendDataRequest() = 0;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) = 0;
public:
	IndependentBranchNode(int Id, QWidget* parent = Q_NULLPTR) 
        : inframedWidget(parent), id(Id), backtracksTo(0) {}
	IndependentBranchNode(int Id, bool iFilter, QWidget* parent = Q_NULLPTR) 
        : inframedWidget(iFilter, parent), id(Id), backtracksTo(0) {}
    int getId() { return id; }
    int& backtracking() {	return backtracksTo;	}
	void processRecord(RecEntity);
	void loadData();
	void setOverloads(const QVector<QueryTemplates::OverloadableQuery>& oqs);
    void throwException(BranchException*);
signals:
	void done(RecEntity);
    void exceptionThrown(BranchException*);
};
