#include "IndependentBranchNode.h"

void IndependentBranchNode::_handleRecord(RecEntity)
{
}


void IndependentBranchNode::processRecord(RecEntity e)
{
	_handleRecord(e);
}

void IndependentBranchNode::loadData()
{
	_sendDataRequest();
}
