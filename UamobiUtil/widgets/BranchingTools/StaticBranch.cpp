#include "StaticBranch.h"
#include "BranchFactory.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void StaticBranch::_allocateNode(BranchDescription desc)
{
	IndependentBranchNode* node = BranchNodeFactory::createNode(desc, this);
	if (node == Q_NULLPTR)
	{
		toNext(RecEntity());
		return;
	}
	createdNodesStack.push_back(node);
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

StaticBranch::StaticBranch(BranchDescription Root, QWidget* parent)
    : AbsBranch(Root, parent), createdNodesStack(), mainLayout(new QVBoxLayout(this))
{
#ifdef DEBUG
	detrace_DCONSTR("StaticBranch");
#endif
	createdNodesStack.reserve(root->count());
	setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
}
