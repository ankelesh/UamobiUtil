#pragma once
#include <QString>
#include <QTextStream>
#include "BranchElementDescription.h"
#include <qstack.h>

/*
	This file contains parser of branch elements. It creates semi-declarative programming
	language which is converted later in non-retrospective tree of independent nodes.
	This is singletone class/

*/


class BranchDescriptionParser
	// converts string into tree of descriptions
{
	QTextStream cin; 
	// accumulated tree is stored in it's root
	BranchDescription root;
	// pointer to current node. Warning, do not use it outside of doParsing.
	BranchElementDescription* currentNode;
	// used for retrospection to avoid "parent" operations in Description object
	QStack<BranchElementDescription* > parentStack;
	// node which is filling right now
	BranchDescription currentToAdd;
	// instanse of this
	static BranchDescriptionParser* _instanse;

	// removes flag and closing symbol
	void _prepareString(QString& buffer);
	// pushes current element into current node, then creates new description
	void _extractAndPush(QString buffer);
	// extracts entity and overwrites it in currentToAdd
	void _extractEntity(QString buffer);
	// extracts overload and pushes into currentToAdd
	void _extractOverload(QString buffer);
	// extracts number of backtracking
	void _extractBacktrack(QString buffer);
	// moves current node selector onto pushed currentToAdd
	void _openSubbranchCapture();
	// moves current node selector onto last description in parent stack
	void _closeSubbranchCapture();
	// extracts name overload and pushes it into currentToAdd
	void _extractNameOverload(QString buffer);
public:
	// you can create another parser, but it is pointless, use instanse
	explicit BranchDescriptionParser();
	// performs parsing over string
	BranchDescription doParsing(QString what);

	static BranchDescriptionParser* instanse();
	// uses instanse to parse string
	static BranchDescription parse(QString what);
};