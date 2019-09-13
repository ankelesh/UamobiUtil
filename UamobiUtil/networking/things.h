#ifndef USER_H
#define USER_H

#include <QStringList>
#include <QDate>
#include <QObject>

struct Answer
{
	enum Statuses {
		Ok        = 200,
		Frobidden = 403,
		NotFound  = 404,
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

struct parsedMode
{
	QString name;
	QString mode;
	QString submode;
};

struct Place : public Answer
{
	QString code;
	QString name;
};
struct parsedPlace
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

struct parsedSupplier
{
	QString code;
	QString name;
	QString orders;
};
struct parsedOrder
{
	QString code;
	QString title;
	QString text;
};
struct DocType : public Answer
{
	DocType(const QString &code, const QString &title)
	{
		this->code  = code;
		this->title = title;
	}

	QString code;
	QString title;
};

struct Document
{
	Document()
		: closed(false), cancelled(false), locked(false)
	{
	}

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

	QString toString(bool sshort = false)
	{
		const QString closed         = QObject::tr("Closed.");
		const QString cancelled      = QObject::tr("Canceled.");
		const QString closedCanceled = QObject::tr("Closed, cancelled.");
		const QString open           = QObject::tr("Open.");
		const QString locked         = QObject::tr("Locked.");

		QString l1, l2, l3;
		l1 = this->parentNr + " > " + this->docId + " — " + this->dateStr;

		if (!supplier.isEmpty())
			l3 = QObject::tr("Supplier: ")+this->supplier;

		if (this->closed && !this->cancelled)
			l2 = closed;
		else if (!this->closed && this->cancelled)
			l2 = cancelled;
		else if (this->closed && this->cancelled)
			l2 = closedCanceled;
		else
			l2 = open;
		if (this->locked)
			l2 += " " + locked;
		return (!sshort ? l1 + "\n" : QString()) + l2 + (l3.isEmpty() ? "" : "\n"+l3);
	}

	QString title()
	{
		return this->parentNr + " > " + this->docId + " — " + this->dateStr;
	}

	QString toStringShort()
	{
		return toString(true);
	}

	QDate date()
	{
		QStringList l = this->dateStr.split(".");
		QDate d;
		if (l.length() == 3)
			d = QDate(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt());
		return d;
	}
};




#endif // USER_H
