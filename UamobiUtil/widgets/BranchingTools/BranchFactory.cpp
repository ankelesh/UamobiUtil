#include "BranchFactory.h"


Branch BranchFactory::createAdjustableBranch(QString description, QWidget* parent)
{
	Branch main(createNWAdjustableBranch(description, parent));
	return main;
}

AbsBranch* BranchFactory::createNWAdjustableBranch(QString description, QWidget* parent)
{
	AbsBranch* main(new AdjustableBranch(parent));
	BranchElements elements = BranchDescriptionParser::parse(description);
	BranchElements::iterator b = elements.begin();
	while (b != elements.end())
	{
		if (b->type == independent_nodes::Subbranch)
		{
			++b;
			continue;
		}
		main->emplaceNode(
			b->type,
			b->oqs,
			b->backtracking,
			b->entity
		);
		++b;
	}
	return main;
}
