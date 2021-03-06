#include "SenderNode.h"
#include "networking/dataupdateengine-http.h"

#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif


void SenderNode::_sendDataRequest()
{

}

void SenderNode::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	if (overloads.isEmpty())
		return;
	QStringList args;
	args << "id" << "title" << "number";
	query = overloads.first().assertedAndMappedCopy(ping, args, args);
}

void SenderNode::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "No entity provided");
#endif
		emit backRequired();
	}
	else
	{
		AppNetwork->execQueryByTemplate(query, e->getId(), e->getTitle(),
			QString::number(e->getAttachedNumber()), Q_NULLPTR);
		emit done(e);
	}
}

SenderNode::SenderNode(QWidget* parent)
	: IndependentBranchNode(independent_nodes::Sender, parent)
{
#ifdef DEBUG
	detrace_DCONSTR("SenderNode");
#endif
}
