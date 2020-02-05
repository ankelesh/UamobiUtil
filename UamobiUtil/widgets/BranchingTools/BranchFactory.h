#pragma once
#include "AdjustableBranch.h"
#include "BranchDescriptionParser.h"
#include "BranchNodeFactory.h"




namespace BranchFactory
{
	Branch createAdjustableBranch(QString desctiption, QWidget* parent = Q_NULLPTR);
	AbsBranch* createNWAdjustableBranch(QString description, QWidget* parent = Q_NULLPTR);
}



