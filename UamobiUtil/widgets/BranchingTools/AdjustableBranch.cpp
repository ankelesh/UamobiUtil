#include "AdjustableBranch.h"

void AdjustableBranch::toNext()
{
	
	if (currentNode < subnodesIds.count() - 1)
	{
		++currentNode;
		_emplaceNodeToCurrent(subnodesIds.at(currentNode));
	}
	else
	{
		emit done(RecEntity());
	}
}

void AdjustableBranch::backtrack()
{
	if (currentNode == 0)
	{
		emit backRequired();
	}
	else if (currentNode < subnodesIds.count())
	{
		int next = currentNode + _upCO<IndependentBranchNode>()->backtracking();
		if (next < 0 || next > subnodesIds.count())
			emit backRequired();
		else
		{
			currentNode = next;
			_emplaceNodeToCurrent(subnodesIds.at(next));
		}
	}
}

void AdjustableBranch::_emplaceNodeToCurrent(int id)
{
	_hideAnyWithDelete(BranchNodeFactory::createNode(
		static_cast<independent_nodes::nodelist>(id),
		backtrackingFlow.at(currentNode),
		queryOverloads.at(currentNode),
		prototypes.at(currentNode),
		this
	));
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &AdjustableBranch::backCalled);
	QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::done, this, &AdjustableBranch::currentNodeDone);
}
