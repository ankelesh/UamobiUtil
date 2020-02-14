#include "StaticBranch.h"
#include "BranchFactory.h"
void StaticBranch::_allocateNode(BranchDescription desc)
{
	createdNodesStack.push_back(BranchNodeFactory::createNode(desc, this));
	if (current == Q_NULLPTR)
		current = createdNodesStack.last();
	mainLayout->addWidget(createdNodesStack.last());
#ifdef QT_VERSION5X
	QObject::connect(createdNodesStack.last(), &inframedWidget::backRequired, this, &StaticBranch::backCalled);
	QObject::connect(createdNodesStack.last(), &IndependentBranchNode::done, this, &StaticBranch::currentNodeDone);
#else
	QObject::connect(createdNodesStack.last(), SIGNAL(backRequired()), this, SLOT(backCalled()));
	QObject::connect(createdNodesStack.last(), SIGNAL(done(RecEntity)), this, SLOT(currentNodeDone(RecEntity)));
#endif
}

void StaticBranch::toNext(RecEntity given)
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

void StaticBranch::backtrack()
{
	if (currentNode == 0)
	{
		emit backRequired();
	}
	else if (currentNode < root->count())
	{
		int next = currentNode + qobject_cast<IndependentBranchNode*>(current)->backtracking();
		if (next < 0 || next > root->count())
			emit backRequired();
		else
		{
			currentNode = next;
			_emplaceNodeToCurrent(root->getNode(next));
		}
	}
	else
	{
		emit done(RecEntity());
	}
}

void StaticBranch::_emplaceNodeToCurrent(BranchDescription desc)
{
	if (currentNode >= createdNodesStack.count())
	{
		_allocateNode(desc);
	}
	_hideCurrent(createdNodesStack.at(currentNode));
}

IndependentBranchNode* StaticBranch::getCurrentNode()
{
	return qobject_cast<IndependentBranchNode*>(current);
}

StaticBranch::StaticBranch(BranchDescription root, QWidget* parent)
	: AbsBranch(root, parent), createdNodesStack(), mainLayout(new QVBoxLayout(this))
{
	createdNodesStack.reserve(root->count());
	setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
}
