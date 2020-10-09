#pragma once
#include "AbsBranch.h"

/*
	This branch is considered as default subbranch type. It allocates it's children 
	dynamically, thus allowing any horizontal length of sequenced nodes. It is
	performing new requests any time it returns back, so if you want to sacrifice 
	length over execution speed, think about StaticBranch.
*/


class AdjustableBranch : public AbsBranch, public abstractDynamicNode
{
	Q_OBJECT
protected:
	// Inherited via AbsBranch
	virtual void toNext(RecEntity) override;
	virtual void backtrack() override;
	virtual void _emplaceNodeToCurrent(BranchDescription id) override;
    virtual IndependentBranchNode* getCurrentNode() override;
    virtual void _handleException(BranchException *) override;
public:
	explicit AdjustableBranch(BranchDescription root, QWidget* parent = Q_NULLPTR);

};
