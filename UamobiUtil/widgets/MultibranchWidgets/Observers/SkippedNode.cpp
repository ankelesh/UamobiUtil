#include "SkippedNode.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
void SkippedNode::_handleRecord(RecEntity e)
{
	emit done(e);
}

void SkippedNode::_sendDataRequest()
{
}

void SkippedNode::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& /*overloads*/)
{
}

SkippedNode::SkippedNode(QWidget* parent) : IndependentBranchNode(independent_nodes::SkipNode,
	parent)
{
#ifdef DEBUG
	detrace_DCONSTR("SkippedNode");
#endif
}
