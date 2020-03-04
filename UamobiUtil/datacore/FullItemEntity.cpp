#include "FullItemEntity.h"
#include "networking/dataupdateengine-http.h"
FullItemEntity::FullItemEntity(QString Title, QString Code, QString Cmid, 
	QString Box, QString Qty, QString Highlight)
	: AbsRecEntity(UniformXmlObject::Item), title(Title), code(Code), cmid(Cmid),
	box(Box), qty(Qty.toInt()), highlight(Highlight.contains("true"))
{
}

void FullItemEntity::sendGetRequest(int pagenumber, RequestAwaiter* awaiter, QString /*doc*/)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::documentGetResults,
		"&page=" + QString::number(pagenumber), "", awaiter
	);
}

void FullItemEntity::sendDeleteThisRequest(RequestAwaiter* awaiter)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::docDeleteByBarcode,
		code, "&qty=" + QString::number(qty),  awaiter);
}

void FullItemEntity::sendDeleteThisRequest(const QueryTemplates::OverloadableQuery& oq, RequestAwaiter* awaiter)
{
	AppNetwork->execQueryByTemplate(oq,
		code, "&qty=" + QString::number(qty), awaiter);
}

QStringList _initFullItemEntityFields()
{
   QStringList t;
    t << "barcode" <<
    "qty" <<
    "title" <<
    "cmid" <<
    "box" <<
    "highlight";
    return t;
}
QStringList _initFullItemEntityDefaults()
{
    QStringList t;
    t << QString() <<
    QString("0") <<
    QString() <<
    QString() <<
    QString() <<
    QString();
    return t;
};

static QStringList FullItemEntityFields(_initFullItemEntityFields());
static QStringList FullItemEntityDefaults(_initFullItemEntityDefaults());

bool FullItemEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Item)
	{
		bool ok;
		QStringList temp = o.mapFields(FullItemEntityFields, FullItemEntityDefaults);
		code = temp.at(0);
        qty = temp.at(1).toDouble(&ok);
		title = temp.at(2);
		cmid = temp.at(3);
		box = temp.at(4);
		highlight = temp.at(5) == "true";
		if (title.isEmpty() || !ok)
			throw InitializationError("title is empty or qty conv failed", " ");
		return true;
	}
	return false;
}

QString FullItemEntity::makeTitle() const
{
	return title;
}

QString FullItemEntity::extractId() const
{
	
	return code;
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
    return static_cast<int>(qty);
}


bool FullItemEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
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

QString ShortItemEntity::extractId() const
{
    return  QString::number(reinterpret_cast<long long int>(this));
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


bool ShortItemEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2)
		return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::documentSearchItems, arguments.at(0), arguments.at(1), awaiter);
	return true;
}
