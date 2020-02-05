#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "networking/QueryTemplates.h"

namespace BranchNodeFactory
{
	IndependentBranchNode* createNode(
		const independent_nodes::nodelist id,
		const int backtracking,
		const QVector<QueryTemplates::OverloadableQuery>& overloads,
		const RecEntity prototype,
		QWidget* parent
	);
};