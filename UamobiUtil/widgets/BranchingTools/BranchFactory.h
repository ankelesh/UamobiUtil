#pragma once
#include "BranchDescriptionParser.h"
#include "BranchElementDescription.h"

class AbsBranch;

/*
	This file contains functions used to create branches or nodes by their types.
	They are reentrant and recursive - if node has subnodes, it will iterate further.
*/


namespace BranchFactory
{
	// creates new branch using it's type. Warning: it will not produce nodes. To create 
	// attached branches use createNode. These functions are created for creating root branches.
	AbsBranch* createNWBranch(QString description, 
		QWidget* parent = Q_NULLPTR, independent_nodes::nodelist branchType = independent_nodes::Subbranch);
	// outdated and renundant
	IndependentBranchNode* createAdjustableBranchAsNode(BranchDescription e,
		QWidget* parent = Q_NULLPTR);
}
namespace BranchNodeFactory
{
	// creates new node using description. Returns nullptr if description had no type.
	IndependentBranchNode* createNode(
		const BranchDescription description,
		QWidget* parent
	);
};


