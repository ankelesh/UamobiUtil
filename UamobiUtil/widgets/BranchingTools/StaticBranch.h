#pragma once
#include "AbsBranch.h"
#include <QBoxLayout>

/*
	This branch is dynamically allocating it's children - but it does not delete it. This
	will save some time during backtracking, but in moving forward it's the same.
*/

class StaticBranch : public AbsBranch, public abstractNode
{
	Q_OBJECT
private:
	QVector<IndependentBranchNode*> createdNodesStack;
	QVBoxLayout* mainLayout;
	
	// creates new node if it does not exists in stack
	void _allocateNode(BranchDescription);
	// Inherited via AbsBranch
	virtual void toNext(RecEntity) override;
	virtual void backtrack() override;
	virtual void _emplaceNodeToCurrent(BranchDescription) override;
	virtual IndependentBranchNode* getCurrentNode() override;
public:
	explicit StaticBranch(BranchDescription root, QWidget* parent = Q_NULLPTR);
};