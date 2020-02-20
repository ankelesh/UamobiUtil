#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "datacore/AbsEntityPrototype.h"


class SkippedNode : public IndependentBranchNode
{
	Q_OBJECT
private:
	// Inherited via IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	SkippedNode(QWidget* parent);

};
