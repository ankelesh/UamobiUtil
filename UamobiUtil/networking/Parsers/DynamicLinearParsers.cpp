#include "DynamicLinearParsers.h"



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
	//detrace_METHCALL("parse_old_api")
	QDomNodeList dmndl = ddoc.elementsByTagName("modes").at(0).childNodes();
	parseres.queriesResult.reserve(dmndl.count() * 2);
	for (int i = 0; i < dmndl.count(); ++i)
	{
		if (dmndl.at(i).nodeName().simplified() == "mode") {
			parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
			//detrace_METHDATAS("parse_old_api", " readed ", << parseres.queriesResult.last())
			parseres.queriesResult << dmndl.at(i).childNodes().at(1).toElement().text();
			//detrace_METHDATAS("parse_old_api", " readed ", << parseres.queriesResult.last())
		}
	}
	parseres.one_position_entries_quantity = 2;
	parseres.alternative_result = 2;
	parseres.type = linear_result;
}

void ModeListParser::parse_new_api(QDomDocument& ddoc)
{
	QDomNodeList dmndl = ddoc.elementsByTagName("modes").at(0).childNodes();
	parseres.queriesResult.reserve(dmndl.count() * 2);
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
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code)
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
