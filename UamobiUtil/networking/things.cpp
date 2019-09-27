#include "things.h"

Document::Document()
	: closed(false), cancelled(false), locked(false)
{
}

QString Document::toString(bool sshort)
{
	const QString closed = QObject::tr("Closed.");
	const QString cancelled = QObject::tr("Canceled.");
	const QString closedCanceled = QObject::tr("Closed, cancelled.");
	const QString open = QObject::tr("Open.");
	const QString locked = QObject::tr("Locked.");

	QString l1, l2, l3;
	l1 = this->parentNr + " > " + this->docId + " — " + this->dateStr;

	if (!supplier.isEmpty())
		l3 = QObject::tr("Supplier: ") + this->supplier;

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
	return (!sshort ? l1 + "\n" : QString()) + l2 + (l3.isEmpty() ? "" : "\n" + l3);
}

QString Document::title()
{
	return this->parentNr + " > " + this->docId + " — " + this->dateStr;
}

QString Document::toStringShort()
{
	return toString(true);
}

QDate Document::date()
{
	QStringList l = this->dateStr.split(".");
	QDate d;
	if (l.length() == 3)
		d = QDate(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt());
	return d;
}

parsedMode::parsedMode(QString n, QString m, QString s)
	: name(n), mode(m), submode(s)
{
}

parsedPlace::parsedPlace(QString Code, QString Name)
	: code(Code), name(Name)
{
}

parsedSupplier::parsedSupplier(QString Code, QString Name, QString Orders)
	: code(Code), name(Name), orders(Orders)
{
}

parsedOrder::parsedOrder(QString Code, QString Title, QString Text)
	: code(Code), title(Title), text(Text)
{
}
