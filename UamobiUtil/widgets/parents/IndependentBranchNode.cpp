#include "IndependentBranchNode.h"


using namespace QueryTemplates;



void IndependentBranchNode::processRecord(RecEntity e)
{
	_handleRecord(e);
}

void IndependentBranchNode::loadData()
{
	_sendDataRequest();
}

void IndependentBranchNode::setOverloads(const QVector<QueryTemplates::OverloadableQuery>& oqs)
{
	_makeOverloads(oqs);
}
