#include "IndependentBranchNode.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

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

void IndependentBranchNode::throwException(BranchException * ex)
{
#ifdef DEBUG
    detrace_METHCALL("IndependentBranchNode::throw");
#endif
    emit exceptionThrown(ex);
}
