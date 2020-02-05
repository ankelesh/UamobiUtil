#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/abstractNodeInterface.h"
#include <QSharedPointer>
#include <Qobject>
#include "networking/dataupdateengine-http.h"


class AbsBranch : public IndependentBranchNode, public abstractDynamicNode
{
	Q_OBJECT
protected:
	QVector< QVector< QueryTemplates::OverloadableQuery> > queryOverloads;
	QVector<int> backtrackingFlow;
	QVector<int> subnodesIds;
	QVector<RecEntity> dependencyTrack;
	Records prototypes;
	int currentNode;

	virtual void toNext() = 0;
	virtual void backtrack() = 0;
	virtual void _emplaceNodeToCurrent(int id) = 0;
	virtual void _sendDataRequest() override;
public:
	explicit AbsBranch(QWidget* parent = Q_NULLPTR);

	void emplaceNode(int node_id, QVector<QueryTemplates::OverloadableQuery>& oqs, int back, RecEntity proto);
	void setBacktracking(int to, int toWhich);
	void setOverload(int toWhich, int index, QueryTemplates::OverloadableQuery oq);
	void raiseThisBranch(RecEntity e);
public slots:
	virtual void backCalled();
	virtual void currentNodeDone(RecEntity given);
};
typedef QSharedPointer<AbsBranch> Branch;



