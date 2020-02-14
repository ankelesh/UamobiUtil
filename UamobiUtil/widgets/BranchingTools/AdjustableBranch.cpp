#include "AdjustableBranch.h"
#include "BranchFactory.h"

void AdjustableBranch::toNext(RecEntity given)
{
	
	if (currentNode < root->count() - 1)
	{
		++currentNode;
		_emplaceNodeToCurrent(root->getNode(currentNode));
		getCurrentNode()->processRecord(given);
		getCurrentNode()->loadData();
		dependencyTrack[currentNode] = RecEntity(given->clone());
	}
	else
	{
		int backtrack = getCurrentNode()->backtracking();
		if (abs(backtrack) > 0)
		{
			backCalled();
			return;
		}
		emit done(given);
	}
}

void AdjustableBranch::backtrack()
{

	if (currentNode == 0)
	{
		emit backRequired();
	}
	else if (currentNode < root->count())
	{
		int next = currentNode + _upCO<IndependentBranchNode>()->backtracking();
		if (next < 0 || next > root->count())
			emit backRequired();
		else
		{
			currentNode = next;
			_emplaceNodeToCurrent(root->getNode(next));
			getCurrentNode()->processRecord(dependencyTrack.at(currentNode));
			getCurrentNode()->loadData();
		}
	}
	else
	{
		emit done(RecEntity());
	}
}

void AdjustableBranch::_emplaceNodeToCurrent(BranchDescription nextNode)
{
	_hideAnyWithDelete( BranchNodeFactory::createNode(
		nextNode,
		this
	));
#ifdef QT_VERSION5X
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &AdjustableBranch::backCalled);
	QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::done, this, &AdjustableBranch::currentNodeDone);
#else
    QObject::connect(currentlyOpened, SIGNAL(backRequired()), this, SLOT(backCalled()));
    QObject::connect(_upCO<IndependentBranchNode>(), SIGNAL(done(RecEntity)), this, SLOT(currentNodeDone(RecEntity)));
#endif
}


IndependentBranchNode* AdjustableBranch::getCurrentNode()
{
	return _upCO<IndependentBranchNode>();
}

AdjustableBranch::AdjustableBranch(BranchDescription root, QWidget* parent)
: AbsBranch(root, parent), 
abstractDynamicNode(Q_NULLPTR, new QVBoxLayout(this)) 
{
	setLayout(mainLayout);
};

