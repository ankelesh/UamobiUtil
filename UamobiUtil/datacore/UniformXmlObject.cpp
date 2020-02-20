#include "UniformXmlObject.h"
#include <QtCore/QHash>



QHash<QString, UniformXmlObject::ThingsIds> _initiateObjectIdHash()
{
	QHash<QString, UniformXmlObject::ThingsIds> t;
	t["place"] = UniformXmlObject::Place;
	t["type"] = UniformXmlObject::Doctype;
	t["doc"] = UniformXmlObject::Document;
	t["result"] = UniformXmlObject::SimpleItem;
	t["mode"] = UniformXmlObject::Mode;
	t["group"] = UniformXmlObject::Group;
	t["groups"] = UniformXmlObject::Group;
	t["order"] = UniformXmlObject::Order;
	t["place"] = UniformXmlObject::Place;
	t["res"] = UniformXmlObject::SimpleItem;
	t["stillages"] = UniformXmlObject::Stillage;
	t["stillage"] = UniformXmlObject::Stillage;
	t["supplier"] = UniformXmlObject::Supplier;
	t["page"] = UniformXmlObject::Page;
	t["control"] = UniformXmlObject::Control;
	t["user"] = UniformXmlObject::User;
	t["parentdoc"] = UniformXmlObject::LesserDocument;
	t["barcodes"] = UniformXmlObject::Barcode;
	return t;
}
QHash<QString, UniformXmlObject::ThingsIds> objectIdLinking(_initiateObjectIdHash());





UniformXmlObject::UniformXmlObject(const int oid)
	: object_id(oid), values()
{
}

UniformXmlObject::UniformXmlObject(const int size, const int oid)
	: object_id(oid), values()
{
	values.reserve(size);
}

UniformXmlObject::UniformXmlObject(const int oid, const QString key, const QString value)
	: object_id(oid), values()
{
	values.insert(key, value);
}

UniformXmlObject::UniformXmlObject(const QHash<QString, QString>& hash,const int oid)
	:object_id(oid), values(hash)
{
}

UniformXmlObject::UniformXmlObject(const QDomNode& dnode)
	: object_id(0), values()
{
	QString nname = dnode.nodeName();
	QDomNodeList insideObject = dnode.childNodes();
	for (int j = 0; j < insideObject.count(); ++j)
		values[insideObject.at(j).nodeName()] = insideObject.at(j).toElement().text();
	object_id = guessObjectId(nname, values.count());
}

UniformXmlObject::UniformXmlObject(const UniformXmlObject::ThingsIds oid, const QDomNode& dnode)
	: object_id(oid), values()
{
	QDomNodeList insideObject = dnode.childNodes();
	for (int j = 0; j < insideObject.count(); ++j)
		values[insideObject.at(j).nodeName()] = insideObject.at(j).toElement().text();
}

const QHash<QString, QString>& UniformXmlObject::directFieldsAccess()
{
	return values;
}

QStringList UniformXmlObject::mapFields(const QStringList fields, const QStringList defaults) const
{
	QStringList result;
	if (fields.count() == defaults.count())
	{
		for (int i = 0; i < fields.count(); ++i)
		{
			result << values.value(fields.at(i), defaults.at(i));
		}
		return result;
	}
	else
	{
		for (int i = 0; i < fields.count(); ++i)
		{
			result << values.value(fields.at(i));
		}
		return result;
	}
}

void UniformXmlObject::addField(const QString fname, const QString fval)
{
	values[fname] = fval;
}

void UniformXmlObject::setFields(const QStringList fields, const QStringList fvalues)
{
	int min = (fields.count() >= fvalues.count()) ? fvalues.count() : fields.count();
	for (int i = 0; i < min; ++i)
	{
		values[fields.at(i)] = fvalues.at(i);
	}
}

const QString UniformXmlObject::value(const QString field) const
{
	return values.value(field);
}

const QString UniformXmlObject::value(const char* field) const
{
	return values.value(QLatin1String(field));
}

void UniformXmlObject::fromNode(const QDomNode& dnode,const int oid)
{
	object_id = oid;
	QDomNodeList dlist = dnode.childNodes();
	for (int i = 0; i < dlist.count(); ++i)
	{
		values[dlist.at(i).nodeName()] = dlist.at(i).toElement().text();
	}
}

bool UniformXmlObject::assertCompatibility(int sz, int oid, bool strict_sizing, int minsz) const
{
	if (oid != 0 || oid != -1)
	{
		if (oid != object_id)
			return false;
	}
	if (strict_sizing)
	{
		return sz == values.count();
	}
	else
	{
		return values.count() >= minsz && values.count() <= sz;
	}
}

bool UniformXmlObject::hasField(const QString field)
{
	return values.contains(field);
}

bool UniformXmlObject::hasField(const char* field)
{
	return values.contains(QLatin1String(field));
}

UniformXmlObject::ThingsIds guessObjectId(QString& oname, int fcount, UniformXmlObject::ThingsIds desired)
{
	UniformXmlObject::ThingsIds temp = objectIdLinking[oname.toLower()];
	switch (temp)
	{
	case UniformXmlObject::SimpleItem:
		if (desired)
			return desired;
		else
			if (fcount > 2)
				return UniformXmlObject::Item;
			else
				return UniformXmlObject::SimpleItem;
		break;
	default:
		return temp;
	}
}
