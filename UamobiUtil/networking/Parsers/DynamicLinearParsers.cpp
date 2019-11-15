#include "DynamicLinearParsers.h"
#define DEBUG

#ifdef DEBUG
#include "debugtrace.h"
#endif
static QVector<QString> _initOpts()
{
	QVector<QString> temp;
	temp.push_back("cmid");
	temp.push_back("qty");
	temp.push_back("box");
	temp.push_back("highlight");
	return temp;
}

static const QVector<QString> itemOptionals(_initOpts());
static const int IOLEN = 4;

bool ModeListParser::couldRead()
{
	return success;
}

bool ModeListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString ModeListParser::parseErrorText()
{
	return errtext;
}
void ModeListParser::parse_old_api(QDomDocument& ddoc)
{
#ifdef DEBUG
	detrace_METHCALL("parse_old_api");
#endif
	QDomNodeList dmndl = ddoc.elementsByTagName("modes").at(0).childNodes();
	parseres.queriesResult.reserve(dmndl.count() * 2);
	for (int i = 0; i < dmndl.count(); ++i)
	{
		if (dmndl.at(i).nodeName().simplified() == "mode") {
			parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
#ifdef DEBUG
			detrace_METHDATAS("parse_old_api", " readed ", << parseres.queriesResult.last());
#endif
			parseres.queriesResult << dmndl.at(i).childNodes().at(1).toElement().text();
#ifdef DEBUG
			detrace_METHDATAS("parse_old_api", " readed ", << parseres.queriesResult.last());
#endif
		}
	}
	parseres.one_position_entries_quantity = 2;
	parseres.alternative_result = 2;
	parseres.type = linear_result;
}

void ModeListParser::parse_new_api(QDomDocument& ddoc)
{
	QDomNodeList dmndl = ddoc.elementsByTagName("modes").at(0).childNodes();
	parseres.queriesResult.reserve(dmndl.count());
	for (int i = 0; i < dmndl.count(); ++i)
	{
		if (dmndl.at(i).nodeName().simplified() == "mode") {
			parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
		}
	}
	parseres.one_position_entries_quantity = 1;
	parseres.alternative_result = 1;
	parseres.type = linear_result;
}

ModeListParser::ModeListParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	if (parseres.request_status != 200)
		return;
	if (doc.elementsByTagName("captionMode").count() > 0)
	{
		parse_old_api(doc);
	}
	else
	{
		parse_new_api(doc);
	}
	success = true;
}

bool ExpandedItemListParser::couldRead()
{
	return success;
}

bool ExpandedItemListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString ExpandedItemListParser::parseErrorText()
{
	return errtext;
}

void ExpandedItemListParser::parse_item_core(const QDomNode ddoc)
{
	QString bc;
	if (ddoc.namedItem("code").isNull())
	{
		if (ddoc.namedItem("barcode").isNull())
		{
			parseres.one_position_entries_quantity = 0;
			return;
		}
		else
		{
			bc = ddoc.namedItem("barcode").toElement().text();
		}
	}
	else
	{
		bc = ddoc.namedItem("code").toElement().text();
	}
	parseres.queriesResult << bc;
	parseres.queriesResult << ddoc.namedItem("title").toElement().text();
}

void ExpandedItemListParser::parse_item_optionals(const QDomNode ddoc)
{
	for (int i = 0; i < IOLEN; ++i)
	{
		if (ddoc.namedItem(itemOptionals[i]).isNull())
		{
			parseres.queriesResult << "";
		}
		else
		{
			parseres.queriesResult << ddoc.namedItem(itemOptionals[i]).toElement().text();
		}
	}
}

ExpandedItemListParser::ExpandedItemListParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	if (parseres.request_status != 200)
		return;
	QDomNodeList pageinfo = doc.elementsByTagName("page");
	if (pageinfo.isEmpty())
	{
		success = false;
		return;
	}
	parseres.queriesResult << pageinfo.at(0).namedItem("from").toElement().text();
	parseres.queriesResult << pageinfo.at(0).namedItem("to").toElement().text();
	parseres.queriesResult << pageinfo.at(0).namedItem("last").toElement().text();
	parseres.alternative_result = 3;
	QDomNodeList dmndl = doc.elementsByTagName("result");
	int len = dmndl.count();
	for (int i = 0; i < len; ++i)
	{
		QDomNode temp = dmndl.at(i);
		parseres.one_position_entries_quantity = 2 + IOLEN;
		parse_item_core(temp);
		parse_item_optionals(temp);
		if (parseres.one_position_entries_quantity == 0)
		{
			success = false;
			return;
		}
	}
	success = true;
}
bool ItemInfoResponseParser::couldRead()
{
	return success;
}

bool ItemInfoResponseParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString ItemInfoResponseParser::parseErrorText()
{
	return errtext;
}

ItemInfoResponseParser::ItemInfoResponseParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(res);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
	if (parseres.request_status != 200)
	{
		success = false;
		return;
	}
	QDomNodeList dmndl = doc.elementsByTagName("nn");
	if (dmndl.count() > 0)
	{
		parseres.queriesResult << "" << "nn" << dmndl.at(0).toElement().text();
	}
	dmndl = doc.elementsByTagName("control");
	QDomNode cntrl;
	for (int i = 0; i < dmndl.count(); ++i)
	{
		cntrl = dmndl.at(i);
#ifdef DEBUG
		detrace_METHEXPL("got control");
#endif
		QDomNodeList insides = cntrl.childNodes();
		parseres.queriesResult << "c";
		for (int j = 0; j < insides.count(); ++j)
		{
#ifdef DEBUG
			detrace_METHEXPL("getting inside: " << insides.at(j).toElement().text());
#endif
			parseres.queriesResult << insides.at(j).toElement().text();
		}
	}
	dmndl = doc.elementsByTagName("richdata");
	if (dmndl.count() > 0)
	{
		parseres.queriesResult << "" << "richdata" << dmndl.at(0).toElement().text();
	}
	else
	{
		parseres.queriesResult << "richdata" << "";
	}
	success = true;
	parseres.one_position_entries_quantity = 3;
}

bool SerializedListParser::couldRead()
{
	return success;
}

bool SerializedListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString SerializedListParser::parseErrorText()
{
	return errtext;
}

SerializedListParser::SerializedListParser(QString& res, QString& err)
	:abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(res);
	QDomNode announcementNode = doc.elementsByTagName("announcement").at(0);
	if (!(announcementNode.namedItem("type").toElement().text() == "list"))
	{
		success = false;
		return;
	}
	bool ok;
	QString intbuffer = announcementNode.namedItem("status").toElement().text();
	parseres.request_status = intbuffer.toInt(&ok);
	if (parseres.request_status != 200 || !ok)
	{
		success = false;
		return;
	}
	intbuffer = announcementNode.namedItem("entrysize").toElement().text();
	parseres.one_position_entries_quantity = intbuffer.toInt(&ok);
	if (!ok)
	{
		success = false;
		return;
	}
	parseres.containingType = announcementNode.namedItem("entryname").toElement().text();
	if (parseres.containingType.isEmpty())
	{
		success = false;
		return;
	}
	QDomNodeList entrylist = doc.elementsByTagName("e");
	QDomNode oneEntry;
	parseres.queriesResult.reserve(entrylist.count() * parseres.one_position_entries_quantity);
	for (int i = 0; i < entrylist.count(); ++i)
	{
		oneEntry = entrylist.at(i);
		for (int j = 0; j < parseres.one_position_entries_quantity; ++j)
		{
			parseres.queriesResult << oneEntry.namedItem("e" + QString::number(j + 1)).toElement().text();
		}
	}
	success = true;
}
