#include "BranchDescriptionParser.h"




QHash<QString, independent_nodes::nodelist> _initBinding()
{
	using namespace independent_nodes;
	QHash<QString, independent_nodes::nodelist> h;
	h["PagedSearch"] = PagedSearch;
	h["FilterSelect"] = FilterSelect;
	h["DocResults"] = DocResults;
	h["ReceiptParameters"] = ReceiptParameters;
	h["NormalScaning"] = NormalScaning;
	h["IdDependentSelect"] = IdDependentSelect;
	h["SelectItemFromList"] = SelectItemFromList;
	h["PlaceSelect"] = PlaceSelect;
	h["InventoryParameters"] = InventoryParameters;
	h["ParentDocument"] = ParentDocument;
	return h;
}

QHash<QString, independent_nodes::nodelist> widgetBinding(_initBinding());

QHash<QString, UniformXmlObject::ThingsIds> bindEntities()
{
	QHash<QString, UniformXmlObject::ThingsIds> h;
	h["Mode"] = UniformXmlObject::Mode;
	h["Place"] = UniformXmlObject::Place;
	h["Supplier"] = UniformXmlObject::Supplier;
	h["Order"] = UniformXmlObject::Order;
	h["Item"] = UniformXmlObject::Item;
	h["SimpleItem"] = UniformXmlObject::SimpleItem;
	h["Document"] = UniformXmlObject::Document;
	h["Doctype"] = UniformXmlObject::Doctype;
	h["Group"] = UniformXmlObject::Group;
	h["Stillage"] = UniformXmlObject::Stillage;
	h["Page"] = UniformXmlObject::Page;
	h["Control"] = UniformXmlObject::Control;
	h["User"] = UniformXmlObject::User;
	h["StatusString"] = UniformXmlObject::StatusString;
	h["LesserDocument"] = UniformXmlObject::LesserDocument;
	h["Richtext"] = UniformXmlObject::Richtext;
	h["Control"] = UniformXmlObject::Control;
	return h;
}
QHash<QString, UniformXmlObject::ThingsIds> entityBinding(bindEntities());


void BranchDescriptionParser::_prepareString(QString& buffer)
{
	buffer = buffer.trimmed();
	buffer.chop(1);
	buffer = buffer.right(buffer.count() - 1);
}

void BranchDescriptionParser::_extractAndPush(QString buffer)
{
	_prepareString(buffer);
	if (current.type != independent_nodes::Subbranch)
	{
		descriptions.push_back(current);
		current = BranchElementDescription();
	}
	current.type = widgetBinding.value(buffer);
}

void BranchDescriptionParser::_extractEntity(QString buffer)
{
	switch (entityBinding.value(buffer))
	{
	case UniformXmlObject::Mode:
		current.entity.reset(new ModeEntity());
			break;
	case UniformXmlObject::Place:
		current.entity.reset(new PlaceEntity());
			break;
	case UniformXmlObject::Supplier:
		current.entity.reset(new SupplierEntity());
			break;
	case UniformXmlObject::Order:
		current.entity.reset(new OrderEntity());
			break;
	case UniformXmlObject::Item:
		current.entity.reset(new FullItemEntity());
			break;
	case UniformXmlObject::SimpleItem:
		current.entity.reset(new ShortItemEntity());
			break;
	case UniformXmlObject::Document:
		current.entity.reset(new FullDocumentEntity());
			break;
	case UniformXmlObject::Doctype:
		current.entity.reset(new DocTypeEntity());
			break;
	case UniformXmlObject::Group:
		current.entity.reset(new GroupEntity());
			break;
	case UniformXmlObject::Stillage:
		current.entity.reset(new StillageEntity());
			break;
	case UniformXmlObject::Control:
		current.entity.reset(new InputControlEntity());
			break;
	case UniformXmlObject::User:
		current.entity.reset(new UserEntity());
			break;
	case UniformXmlObject::LesserDocument:
		current.entity.reset(new LesserDocumentEntity());
			break;
	default:
		return;
	}
}

void BranchDescriptionParser::_extractOverload(QString buffer)
{
	using namespace QueryTemplates;
	_prepareString(buffer);
	if (buffer.isEmpty())
	{
		current.oqs.push_back(OverloadableQuery::defaultQuery());
		return;
	}
	QVector<QStringRef> temp = buffer.splitRef(" : ");
	if (temp.count() != 2)
	{
		if (temp.count() == 1)
			current.oqs.push_back(OverloadableQuery(temp.at(0).toString()));
		return;
	}
	QVector<QStringRef> args(temp.at(1).split(" , "));
	current.oqs.push_back(OverloadableQuery(temp.first(), args));
}

void BranchDescriptionParser::_extractBacktrack(QString buffer)
{
	_prepareString(buffer);
	bool ok;
	int backtrack = buffer.toInt(&ok);
	if (!ok)
		return;
	current.backtracking = backtrack;
}

BranchDescriptionParser::BranchDescriptionParser()
	: cin(), descriptions(), current()
{
}

QVector<BranchElementDescription> BranchDescriptionParser::doParsing(QString what)
{
	descriptions.clear();
	cin.setString(&what, QIODevice::ReadOnly);
	QString buffer;
	while (true)
	{
		cin.readLineInto(&buffer);
		if (cin.atEnd())
		{
			if (current.type != independent_nodes::Subbranch)
				descriptions.push_back(current);
			break;
		}
		if (buffer.isEmpty())
			continue;
		switch (buffer.at(0).toLatin1())
		{
		case '[':
			_extractAndPush(buffer);
			break;
		case '%':
			_extractEntity(buffer);
			break;
		case '<':
			_extractOverload(buffer);
			break;
		case '*':
			_extractBacktrack(buffer);
			break;
		default:
			break;
		}
	}
	return descriptions;
}

BranchDescriptionParser* BranchDescriptionParser::_instanse = Q_NULLPTR;
BranchDescriptionParser* BranchDescriptionParser::instanse()
{
	if (_instanse == Q_NULLPTR)
		_instanse = new BranchDescriptionParser();
	return _instanse;
}

QVector<BranchElementDescription> BranchDescriptionParser::parse(QString what)
{
	return instanse()->doParsing(what);
}

BranchElementDescription::BranchElementDescription(independent_nodes::nodelist Type,
	QVector<QueryTemplates::OverloadableQuery> Oqs, int Backtracking, RecEntity Entity)
	: type(Type), oqs(Oqs), backtracking(Backtracking), entity(Entity)
{
}
