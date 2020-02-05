#include "AbsBranch.h"

AbsBranch::AbsBranch(QWidget* parent)
	: IndependentBranchNode(independent_nodes::Subbranch, parent),
	abstractDynamicNode(new inframedWidget(this),
		new QVBoxLayout(this)),  queryOverloads(), backtrackingFlow(),
	subnodesIds(), currentNode(0)
{
}

void AbsBranch::emplaceNode(int node_id, QVector<QueryTemplates::OverloadableQuery>& oqs, int back, RecEntity record)
{
	queryOverloads.push_back(oqs);
	backtrackingFlow.push_back(back);
	subnodesIds.push_back(node_id);
	prototypes.push_back(record);
	dependencyTrack.push_back(RecEntity());
}

void AbsBranch::setBacktracking(int to, int toWhich)
{
	if (toWhich >= 0 || toWhich < backtrackingFlow.count())
		backtrackingFlow[toWhich] = to;
}

void AbsBranch::setOverload(int toWhich, int index, QueryTemplates::OverloadableQuery oq)
{
	if (toWhich >= 0 || toWhich < queryOverloads.count())
		if (index >= 0 || index < queryOverloads.at(toWhich).count())
			queryOverloads[toWhich][index] = oq;
}

void AbsBranch::raiseThisBranch(RecEntity starter)
{
	currentNode = -1;
	toNext();
	_upCO<IndependentBranchNode>()->processRecord(starter);
	_upCO<IndependentBranchNode>()->loadData();
	dependencyTrack[currentNode].reset(starter->clone());

}

void AbsBranch::backCalled()
{
	if (currentNode == 0)
	{
		emit backRequired();
	}
	else if (currentNode < subnodesIds.count())
	{
		int next = currentNode + (_upCO<IndependentBranchNode>()->backtracking()-1);
		if (next < 0 || next > subnodesIds.count())
			emit backRequired();
		else
		{
			currentNode = next;
			_emplaceNodeToCurrent(subnodesIds.at(next));
			_upCO<IndependentBranchNode>()->loadData();
			_upCO<IndependentBranchNode>()->processRecord(dependencyTrack.at(next));
		}
	}
}

void AbsBranch::currentNodeDone(RecEntity given)
{
	if (currentNode < subnodesIds.count() - 1)
	{
		++currentNode;
		_emplaceNodeToCurrent(subnodesIds.at(currentNode));
		_upCO<IndependentBranchNode>()->processRecord(given);
		_upCO<IndependentBranchNode>()->loadData();
		dependencyTrack[currentNode].reset(given->clone());
	}
	else
	{
		emit done(given);
	}
}

void AbsBranch::_sendDataRequest()
{
}
