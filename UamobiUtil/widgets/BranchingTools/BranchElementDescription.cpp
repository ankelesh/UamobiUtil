#include "BranchElementDescription.h"


BranchElementDescription::BranchElementDescription(independent_nodes::nodelist Type,
	QVector<QueryTemplates::OverloadableQuery> Oqs, int Backtracking, RecEntity Entity, QStringList no)
	: subElements(), type(Type), oqs(Oqs), backtracking(Backtracking), entity(Entity), namesOverload(no)
{
}

void BranchElementDescription::emplaceNode(BranchDescription& node)
{
	subElements.push_back(node);
    node = BranchDescription(new BranchElementDescription());
}
void BranchElementDescription::emplaceNode(const BranchDescription& node)
{
	subElements.push_back(BranchDescription(node->clone()));
}
BranchDescription& BranchElementDescription::getNode(int index)
{
	if (index >= 0 && index < subElements.count())
	{
		return subElements[index];
	}
	else
		return subElements.first();
}

BranchDescription& BranchElementDescription::first()
{
	return subElements.first();
}

const BranchDescription& BranchElementDescription::first() const
{
	return subElements.first();
}

BranchDescription& BranchElementDescription::last()
{
	return subElements.last();
}

const BranchDescription& BranchElementDescription::last() const
{
	return subElements.last();
}

BranchElements& BranchElementDescription::extractDesctiptions()
{
	return subElements;
}

const BranchElements& BranchElementDescription::extractDesctiptions() const
{
	return subElements;
}



bool BranchElementDescription::hasSubElements() const
{
	return !subElements.isEmpty();
}

bool BranchElementDescription::isEmpty() const
{
	return subElements.isEmpty();
}

int BranchElementDescription::count() const
{
	return subElements.count();
}

const BranchDescription& BranchElementDescription::at(int index) const
{
	return subElements.at(index);
}

BranchDescription BranchElementDescription::clone() const
{
	BranchDescription desc(
		new BranchElementDescription(type, oqs, backtracking, RecEntity(entity->clone()))
	);
	for (int i = 0; i < subElements.count(); ++i)
	{
		desc->emplaceNode(subElements.at(i)->clone());
	}
	return desc;

}
