#pragma once
#include <QString>
#include <QTextStream>
#include "widgets/parents/IndependentBranchNode.h"
#include "networking/QueryTemplates.h"

struct BranchElementDescription
{
	independent_nodes::nodelist type;
	QVector<QueryTemplates::OverloadableQuery> oqs;
	int backtracking;
	RecEntity entity;
	BranchElementDescription(independent_nodes::nodelist type = independent_nodes::Subbranch,
		QVector<QueryTemplates::OverloadableQuery> oqs = QVector<QueryTemplates::OverloadableQuery>(),
		int backtracking = 0, RecEntity entity = RecEntity());
};
typedef QVector<BranchElementDescription> BranchElements;


class BranchDescriptionParser
{
	QTextStream cin; 
	QVector<BranchElementDescription> descriptions;
	BranchElementDescription current;
	static BranchDescriptionParser* _instanse;
	void _prepareString(QString& buffer);
	void _extractAndPush(QString buffer);
	void _extractEntity(QString buffer);
	void _extractOverload(QString buffer);
	void _extractBacktrack(QString buffer);
public:
	explicit BranchDescriptionParser();
	QVector<BranchElementDescription> doParsing(QString what);
	static BranchDescriptionParser* instanse();
	static QVector<BranchElementDescription> parse(QString what);
};