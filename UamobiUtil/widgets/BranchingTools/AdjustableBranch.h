#pragma once
#include "AbsBranch.h"
#include "BranchNodeFactory.h"



class AdjustableBranch : public AbsBranch
{
	Q_OBJECT
protected:
	virtual void toNext() override;
	virtual void backtrack() override;
	virtual void _emplaceNodeToCurrent(int id) override;
public:
	explicit AdjustableBranch(QWidget* parent = Q_NULLPTR) : AbsBranch(parent) {};
};