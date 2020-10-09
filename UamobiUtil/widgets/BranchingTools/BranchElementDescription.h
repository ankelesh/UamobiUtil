#pragma once
#include <QtCore/QVector>
#include "datacore/AbsEntityPrototype.h"
#include "networking/QueryTemplates.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/BranchingTools/BranchException.h"

/*
	This class represents node in widget tree. It DOES NOT contains widget itself,
	but you can create node using information from it. Check BranchFactory file.
	It is forward-only, you can not find it's parent using it's interfaces. This was done so 
	because actual IndependentNodes does not operate over parents.
	Warning: always clone descriptions if you want to insert them into other node to avoid 
	(or at least find) circular dependencies. All trees produced by parser are 100% clear.
*/

class BranchElementDescription
{
	typedef QSharedPointer<BranchElementDescription> BranchDescription;
	typedef QVector<BranchDescription> BranchElements;
private:
	// children elements are private because their implementation can be changed
	BranchElements subElements;
public:
	// these parameters are used in createNode()

	// type of node (actual type of widget that must be constructed
	independent_nodes::nodelist type;
	// queries to overload in final widget
	QVector<QueryTemplates::OverloadableQuery> oqs;
	// coefficient affects how long will be jump back
	int backtracking;
	// prototype for prototype-dependent widgets
	RecEntity entity;
	// overloads of names used in widget
	QStringList namesOverload;

	BranchElementDescription(independent_nodes::nodelist type = independent_nodes::NotANode,
		QVector<QueryTemplates::OverloadableQuery> oqs = QVector<QueryTemplates::OverloadableQuery>(),
		int backtracking = 0, RecEntity entity = RecEntity(),QStringList namesoverload = QStringList());

	// tree node interfaces
	
	// inserts or clones node (depends if it's const)
	void emplaceNode(BranchDescription& node);
	void emplaceNode(const BranchDescription& node);

	// returns node on index
	BranchDescription& getNode(int index);
	BranchDescription& first();
	const BranchDescription& first() const;
	BranchDescription& last();
	const BranchDescription& last() const;
	// returns link to subelements. Deprecated.
	BranchElements& extractDesctiptions();
	const BranchElements& extractDesctiptions() const;
	// checks of subelement vector
	bool hasSubElements() const;
	bool isEmpty() const;
	int count() const;
	const BranchDescription& at(int index) const;
	QString describe();
	// returns new description with full copy of it's children
	BranchDescription clone() const;
};
typedef QSharedPointer<BranchElementDescription> BranchDescription;
typedef QVector<BranchDescription> BranchElements;
