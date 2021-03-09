#pragma once
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtXml/QDomNode>
#include <QtCore/qsharedpointer.h>
#include <QtCore/QVector>

/*
	This object is imitating Json objects - it holds field hash and id of stored object. 
	Id is required for safer extraction - it blocks 100% non-compatible objects initialization.
*/
class UniformXmlObject
	// imitates JSON object
{
public:
	enum ThingsIds
		// used as predefined values for oid checks
	{
		NotAThing,
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
		Control,
		Barcode,
		Invoice,
		EntityList
	};

private:
	// used for compatibility checks
	int object_id;
	// stores actual fields of an object
	QHash<QString, QString> values;

	void _extractXMLNode(const QDomNode& dnode);
public:
	explicit UniformXmlObject(const int oid = -1);
	explicit UniformXmlObject(const int size, const int oid);
	UniformXmlObject(const int oid, const  QString key, const QString value);
	explicit UniformXmlObject(const QHash<QString, QString>& hash, const int oid = -1);
	explicit UniformXmlObject(const QDomNode& dnode);
	UniformXmlObject(const ThingsIds oid, const QDomNode& dnode);

	// allows to access fields in read only.
	const QHash<QString, QString>& directFieldsAccess();
	// extracts values with defaults. If no defaults is provided, execution can be interrupted
	QStringList mapFields(const QStringList fields, const QStringList defaults = QStringList()) const;
	// inserts new field
	void addField(const QString fname, const QString fval);
	// resets fields
	void setFields(const QStringList fields, const QStringList values = QStringList());
	// returns value copy by field
	const QString value(const QString field) const;
	const QString value(const char* field) const;
	// extracts object from node with possibility to override oid
	void fromNode(const QDomNode& dnode, int oid = -1);
	// getters
    int mySize() const { return values.count(); }
    int myOID() const { return object_id; }
	// checks full compatibility - both objects must has same size, fields and object ids
	bool assertCompatibility(int sz, int oid, bool strict_sizing = true, int minsz = 0) const;
	// key check
	bool hasField(const QString field);
	bool hasField(const char* field);
	// renames field if one exists. May be usefull
	void renameField(const QString oldname, const QString newName);
};


// guesses object id using name extracted from node.
UniformXmlObject::ThingsIds guessObjectId(QString oname,
	int fcount = 0, UniformXmlObject::ThingsIds desired = UniformXmlObject::NotAThing);



typedef QSharedPointer<UniformXmlObject> XmlObject;
typedef QVector< QSharedPointer< UniformXmlObject> > XmlObjects;
