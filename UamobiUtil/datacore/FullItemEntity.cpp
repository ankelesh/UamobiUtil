#include "FullItemEntity.h"
#include "networking/dataupdateengine-http.h"
FullItemEntity::FullItemEntity(QString Title, QString Code, QString Cmid, 
	QString Box, QString Qty, QString Highlight)
	: AbsRecEntity(UniformXmlObject::Item), title(Title), code(Code), cmid(Cmid),
	box(Box), qty(Qty.toInt()), highlight(Highlight.contains("true"))
{
}

void FullItemEntity::sendGetRequest(int pagenumber, RequestAwaiter* awaiter, QString doc)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::documentGetResults,
		"&page=" + QString::number(pagenumber), doc, awaiter
	);
}

void FullItemEntity::sendDeleteThisRequest(RequestAwaiter* awaiter)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::docDeleteByBarcode,
		code, "&qty=" + qty, awaiter);
}


QStringList FullItemEntityFields
{
	"barcode",
	"qty",
	"title",
	"cmid",
	"box",
	"highlight"
};
QStringList FullItemEntityDefaults
{
	QString(),
	QString(),
	QString(),
	QString(),
	QString(),
	QString()
};

bool FullItemEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Item)
	{
		QStringList temp = o.mapFields(FullItemEntityFields, FullItemEntityDefaults);
		if (title.isEmpty())
			throw InitializationError("title", " ");
		return true;
	}
	return false;
}

QString FullItemEntity::makeTitle() const
{
	return title;
}

IdInt FullItemEntity::extractId() const
{
	bool ok;
	IdInt cd = code.toLongLong(&ok);
	if (!ok)
		return (IdInt)(this);
	return cd;
}

bool FullItemEntity::deepCompare(const AbsRecEntity* another) const
{
	const FullItemEntity* temp = dynamic_cast<const FullItemEntity*>(another);
	if (temp == Q_NULLPTR)
		return false;
	return (class_id == another->myType()) && (temp->code == code);
}

AbsRecEntity* FullItemEntity::fabricate() const
{
	return new FullItemEntity(*this);
}

bool FullItemEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this > another;
}

bool FullItemEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this > &(*another);
}

int FullItemEntity::extractEnumerable() const
{
	return qty;
}


bool FullItemEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2)
		return false;
	sendGetRequest(arguments.at(0).toInt(),awaiter, arguments.at(1));
	return true;
}

ShortItemEntity::ShortItemEntity(QString Code, QString Name)
	: AbsRecEntity(UniformXmlObject::SimpleItem), code(Code), name(Name)
{
}

bool ShortItemEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::SimpleItem)
	{
		name = o.value("title");
		code = o.value("code");
		if (name.isEmpty())
			throw InitializationError("title", " ");
		return true;
	}
	return false;
}

QString ShortItemEntity::makeTitle() const
{
	return name;
}

IdInt ShortItemEntity::extractId() const
{
	return (IdInt)this;
}

bool ShortItemEntity::deepCompare(const AbsRecEntity* another) const
{
	const ShortItemEntity* temp = dynamic_cast<const ShortItemEntity*>(another);
	if (temp == Q_NULLPTR)
		return false;
	return (class_id == another->myType()) && (temp->code == code);
}

AbsRecEntity* ShortItemEntity::fabricate() const
{
	return new ShortItemEntity(*this);
}

bool ShortItemEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this > another;
}

bool ShortItemEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this > &(*another);
}


bool ShortItemEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2)
		return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::documentSearchItems, arguments.at(0), arguments.at(1), awaiter);
	return true;
}
