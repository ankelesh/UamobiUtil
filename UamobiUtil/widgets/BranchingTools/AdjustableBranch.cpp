#include "AdjustableBranch.h"
#include "BranchFactory.h"
#include "widgets/BranchingTools/BranchException.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif

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
		if (currentlyOpened == Q_NULLPTR && !root->hasSubElements())
		{
			emit done(given);
			return;
		}
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
		int next = currentNode + _upCO<IndependentBranchNode>()->backtracking() - 1;
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
	IndependentBranchNode* node = BranchNodeFactory::createNode(
		nextNode,
		this
	);
	if (node == Q_NULLPTR)
	{
		raiseThisBranch(RecEntity());
		return;
	}
	_hideAnyWithDelete(node);
	
#ifdef QT_VERSION5X
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &AdjustableBranch::backCalled);
	QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::done, this, &AdjustableBranch::currentNodeDone);
    bool a = QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::exceptionThrown, this, &AbsBranch::handleException);
#else
    QObject::connect(currentlyOpened, SIGNAL(backRequired()), this, SLOT(backCalled()));
    QObject::connect(_upCO<IndependentBranchNode>(), SIGNAL(done(RecEntity)), this, SLOT(currentNodeDone(RecEntity)));
    QObject::connect(_upCO<IndependentBranchNode>(), SIGNAL(exceptionThrown(BranchException*)), this, SLOT(handleException(BranchException*)));
#endif
}


IndependentBranchNode* AdjustableBranch::getCurrentNode()
{
	return _upCO<IndependentBranchNode>();
}

AdjustableBranch::AdjustableBranch(BranchDescription Root, QWidget* parent)
: AbsBranch(Root, parent),
abstractDynamicNode(Q_NULLPTR, new QVBoxLayout(this)) 
{
#ifdef DEBUG
	detrace_DCONSTR("Adjustable branch with " << root->count() << " children");
#endif
	setLayout(mainLayout);
};



void AdjustableBranch::_handleException(BranchException * ex)
{
    if (ex == Q_NULLPTR)
    {
        return;
    }
    if (ex->isDirectionedOutside())
    {
        throwException(ex);
        return;
    }
    if (ex->isDirectioned())
    {
        if (ex->whereToReturn() >= 0 && ex->whereToReturn() < root->count())
        {
            currentNode = ex->whereToReturn();
            _emplaceNodeToCurrent(root->getNode(ex->whereToReturn()));
            getCurrentNode()->processRecord(dependencyTrack.at(currentNode));
            getCurrentNode()->loadData();
            delete ex;
            return;
        }
        else
            throwException(ex);
    }
    else
    {
        for (int i = currentNode; i >= 0; --i)
        {
            if (ex->canReturnHere(root->at(i)->type))
            {
                currentNode = i;
                _emplaceNodeToCurrent(root->getNode(currentNode));
                getCurrentNode()->processRecord(dependencyTrack.at(currentNode));
                getCurrentNode()->loadData();
                delete ex;
                return;
            }
        }
        throwException(ex);
    }
}
