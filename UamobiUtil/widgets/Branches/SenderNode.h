#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "networking/RequestAwaiter.h"
#include "networking/things.h"

/*
	This node is simple no-reaction-sender which can be added to branch for out-of-line requests
*/

class SenderNode : public IndependentBranchNode
{
protected:
	QueryTemplates::OverloadableQuery query;
	// Inherited via IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	SenderNode(QWidget* parent);

};