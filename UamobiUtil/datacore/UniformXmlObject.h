#pragma once
#include <QStringList>
#include <QString>
#include <QHash>
#include <QtXml/QDomNode>
#include <QtCore/qsharedpointer.h>

int guessObjectId(QString& oname);





class UniformXmlObject
{
public:
	enum ThingsIds
	{
		Mode,
		Place,
		Supplier,
		Order,
		Item,
		SimpleItem,
		Document,
		Doctype,
		Group,
		Stillage,
		Page,
		User,
		StatusString,
		LesserDocument,
		Richtext,
		Control
	};

private:
	int object_id;
	QHash<QString, QString> values;
public:
	explicit UniformXmlObject(const int oid = -1);
	explicit UniformXmlObject(const int size, const int oid);
	UniformXmlObject(const int oid, const  QString key, const QString value);
	explicit UniformXmlObject(const QHash<QString, QString>& hash, const int oid = -1);
	explicit UniformXmlObject(const QDomNode& dnode);
	UniformXmlObject(const ThingsIds oid, const QDomNode& dnode);
	const QHash<QString, QString>& directFieldsAccess();
	QStringList mapFields(const QStringList fields, const QStringList defaults = QStringList()) const;
	void addField(const QString fname, const QString fval);
	void setFields(const QStringList fields, const QStringList values = QStringList());
	const QString value(const QString field) const;
	const QString value(const char* field) const;
	void fromNode(const QDomNode& dnode, int oid = -1);
	int mySize() const { return values.count(); };
	int myOID() const { return object_id; };
	bool assertCompatibility(int sz, int oid, bool strict_sizing = true, int minsz = 0) const;
	bool hasField(const QString field);
	bool hasField(const char* field);
};


typedef QSharedPointer<UniformXmlObject> XmlObject;
typedef QVector< QSharedPointer< UniformXmlObject> > XmlObjects;
