#ifndef USER_H
#define USER_H

#include <QtCore/QStringList>
#include <QtCore/QDate>
#include <QtCore/QObject>

/*
		This file contains definitions for structures used in the application. It is kinda too functional
		approach, but this legacy thing is needed. Later these structures will be morphed into classes, except legacy ones.

*/

struct parsedMode
	// Mode, which was parsed from netquery
{
	QString name;	//	name
	QString mode;	//	mode - larger part
	QString submode;	//	submode - sometimes is 0
	parsedMode(QString Name = "", QString Mode = "", QString Submode = "");
	QString debugSnapshot();
};

struct parsedPlace
	// Place parsed from netquery
{
	QString code;	//	code, used in queries
	QString name;	//	name which is shown to user
	parsedPlace(QString Code = "", QString Name = "");
};

struct parsedSupplier
	// Supplier parsed from netquery
{
	QString code;	//	code, used in queries
	QString name;	//	name which is shown to user
	QString orders;	//	orders, which are used for various actions
	parsedSupplier(QString Code = "", QString Name = "", QString Orders = "");
};
struct parsedOrder
	// order parsed from netquery
{
	QString code;	//	code, used in queries
	QString title;	//	title, used as title
	QString text;	//	richtext, used to create label view
	parsedOrder(QString Code = "", QString Title = "", QString Text = "");
};
struct parsedItem
{
	QString title;
	QString code;
	QString cmid;
	QString box;
	int qty;
	bool highlight;
	parsedItem(QString title = "", QString code = "", QString cmid = "", QString box = "", QString qty = "", QString highlight = "");
	QString description() const;
};
struct parsedItemSimplified
{
	QString barcode;
	QString title;
	parsedItemSimplified(QString Barcode = "", QString title = "");
	QString description() const;
};

struct parsedDocument
{
	QString code;
	QString title;
	QString text;
	QString doctype;
	parsedDocument(QString code = "", QString title = "", QString text = "", QString doctype = "");
	QString description() const;
};
struct parsedDocType
{
	QString id;
	QString name;
	bool notFiltered;
	parsedDocType(QString id = "", QString name = "", QString isF = "");
	QString description() const;
};
//			HERE START LEGACY STRUCTURES. DO NOT TOUCH THEM - INSTEAD CREATE NEW, BETTER ONES		//
struct Answer
{
	enum Statuses {
		Ok = 200,
		Frobidden = 403,
		NotFound = 404,
		InternalError = 500
	};

	Statuses status;
};
struct UserProfile : public Answer
{
	QString login;
	QString name;
	QString place;

	void clear()
	{
		login.clear();
		name.clear();
		place.clear();
	}
};
struct Mode : public Answer
{
	QString name;  // служебное наименование, используется внутри проекта;
	QString caption;  // пользовательское наименование, установка из конфигуратора - св-во CaptionMode;
};
struct Place : public Answer
{
	QString code;
	QString name;
};
struct DocResultItem : public Answer
{
	QString code;
	QString title;
	float num;
};
struct DocType : public Answer
{
	DocType(const QString& code, const QString& title)
	{
		this->code = code;
		this->title = title;
	}

	QString code;
	QString title;
};
struct Document
{
	Document();

	QString docId;
	QString parentNr;
	QString dateStr;
	QString comment;
	QString supplier;
	//	Q_DECL_DEPRECATED int scanMode;
	bool inspect;
	bool closed;
	bool cancelled;
	bool locked;

	QString toString(bool sshort = false);

	QString title();

	QString toStringShort();

	QDate date();
};

#endif // USER_H
