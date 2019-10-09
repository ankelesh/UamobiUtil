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

parsedItemSimplified::parsedItemSimplified(QString Barcode, QString Title)
	: barcode(Barcode), title(Title)
{
}

QString parsedItemSimplified::description() const
{
	return title + " " + barcode;
}

parsedItem::parsedItem(QString Code, QString Title, QString Cmid, QString Qty, QString Box, QString Highlight)
	: title(Title), code(Code), cmid(Cmid), box(Box), qty(0), highlight(false)
{
	bool ok;
	qty = Qty.toInt(&ok);
	if (!ok)
		qty = 0;
	if (Highlight.contains("high"))
		highlight = true;
}

QString parsedItem::description() const
{
	return title + "|bc: " + code +" | "+ cmid + "|bx: " +  box + " qty: " + QString::number(qty) + " highlight: " + ((highlight)? "true" : "false");
}
