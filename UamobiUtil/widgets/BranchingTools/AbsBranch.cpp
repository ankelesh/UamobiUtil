#include "AbsBranch.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"

AbsBranch::AbsBranch(QWidget* parent)
	: IndependentBranchNode(independent_nodes::Subbranch, parent),
	 root(new BranchElementDescription()), currentNode(0)
{
	stopListeningKeyboard();
}

AbsBranch::AbsBranch(BranchDescription Root, QWidget* parent)
	: IndependentBranchNode(independent_nodes::Subbranch, parent),
    root(Root), dependencyTrack(Root->count()),currentNode(0)
{
}

void AbsBranch::emplaceNode(int node_id, QVector<QueryTemplates::OverloadableQuery>& oqs, int back, RecEntity record)
{
	root->emplaceNode(
		BranchDescription(
			new BranchElementDescription(
				static_cast<independent_nodes::nodelist>(node_id), oqs, back, record
			)));
	dependencyTrack.push_back(RecEntity());
}

void AbsBranch::emplaceNode(const BranchDescription e)
{
	root->emplaceNode(e);
	dependencyTrack.push_back(RecEntity());
}


void AbsBranch::setBacktracking(int to, int toWhich)
{
	if (toWhich >= 0 || toWhich < root->count())
		root->getNode(toWhich)->backtracking = to;
}

void AbsBranch::setOverload(int toWhich, int index, QueryTemplates::OverloadableQuery oq)
{
	if (toWhich >= 0 || toWhich < root->count())
		if (index >= 0 || index < root->getNode(toWhich)->oqs.count())
			root->getNode(toWhich)->oqs[index] = oq;
}

void AbsBranch::raiseThisBranch(RecEntity starter)
{
	currentNode = -1;
	toNext(starter);
}

void AbsBranch::backCalled()
{
	backtrack();
}

void AbsBranch::currentNodeDone(RecEntity given)
{
    toNext(given);
}

void AbsBranch::handleException(BranchException *ex)
{
    _handleException(ex);
}

void AbsBranch::_handleRecord(RecEntity e)
{
	raiseThisBranch(e);
}

void AbsBranch::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& /*overloads*/)
{
}

void AbsBranch::listenKeyboard()
{

}

void AbsBranch::_sendDataRequest()
{
}
