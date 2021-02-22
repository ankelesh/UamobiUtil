#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include <QSharedPointer>
#include <QObject>
#include "networking/dataupdateengine-http.h"
#include "BranchElementDescription.h"


/*
	This class is basic for most global branches (global branch means it can be any lenght,
	while local branches are cyclic or very short). It can manage all abstract logic
	itself, while leaving details of how to emplace node or go to another widget to children.
	Check AdjustableBranch and StaticBranch to see full implementation. Also it implements 
	slot interfaces.
*/


class AbsBranch : public IndependentBranchNode
{
	Q_OBJECT
protected:
	// tree of all contents
	BranchDescription root;
	// this vector stores dependencies to keep all entity-dependent nodes behaviour untouched
	// (they will be refreshed as normal)
	QVector<RecEntity> dependencyTrack;
	// index of current node, used to control flow
	int currentNode;

	// must implement opening next node (current + 1)
	virtual void toNext(RecEntity) = 0;
	// must implement opening previous node (current - 1)
	virtual void backtrack() = 0;
	// must emplace node. For dynamic branches this usually means new allocation,
	// for static branches - cache operations or just skipping.
    virtual void _emplaceNodeToCurrent(BranchDescription) = 0;
	// Must return current node
	virtual IndependentBranchNode* getCurrentNode() = 0;
	// Inherited via IndependentBranchNode
	// does nothing
	virtual void _sendDataRequest() override;
	// raises branch, using entity as starter
	virtual void _handleRecord(RecEntity) override;
	// does nothing
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
    virtual void _handleException(BranchException*) = 0;
	virtual void listenKeyboard() override;
public:
	// constructs empty branch
	explicit AbsBranch(QWidget* parent = Q_NULLPTR);
	// constructs ready-to-go branch using root element
	explicit AbsBranch(BranchDescription root, QWidget* parent = Q_NULLPTR);
	// inserts new node into flow, by creating description
	void emplaceNode(int node_id, QVector<QueryTemplates::OverloadableQuery>& oqs, int back, RecEntity proto);
	// inserts new node by copy of other description
	void emplaceNode(const BranchDescription e);
	// methods for forced setup of backtracking and overloads
	void setBacktracking(int to, int toWhich);
	void setOverload(int toWhich, int index, QueryTemplates::OverloadableQuery oq);
	// restarts branch
	void raiseThisBranch(RecEntity e);

public slots:
	// these slots are wrapping toNext and backtracking interfaces
	void backCalled();
	void currentNodeDone(RecEntity given);
    void handleException(BranchException* ex);

};
typedef QSharedPointer<AbsBranch> Branch;



