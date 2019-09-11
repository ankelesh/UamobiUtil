#include "SimpleResponseParsers.h"

bool SimpliestResponceParser::couldRead()
{
	return success;
}

bool SimpliestResponceParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString SimpliestResponceParser::parseErrorText()
{
	return errtext;
}

SimpliestResponceParser::SimpliestResponceParser(QString& res, QString& err)
	:	abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	try
	{
		QString code = doc.elementsByTagName("status").at(0).toElement().text();
		parseres.request_status = code.toInt();
		if (parseres.request_status != 200)
		{
            QDomNodeList li = doc.elementsByTagName("message");
			if (li.count() > 0)
				parseres.queriesResult.push_back(
					li.at(0).toElement().text()
				);
			li = doc.elementsByTagName("stack");
			if (li.count() > 0)
				parseres.queriesResult.push_back(
					li.at(0).toElement().text()
				);
		}
		if (doc.elementsByTagName("session").at(0).toElement().text().toInt() != 0)
			parseres.alternative_result = 1;
		success = true;
	}
	catch (...)
	{
		success = false;
	}
}

bool RichtextResponseParser::couldRead()
{
	return success;
}

bool RichtextResponseParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString RichtextResponseParser::parseErrorText()
{
	return errtext;
}

RichtextResponseParser::RichtextResponseParser(QString& res, QString& err)
	:abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	try
	{
		QString code = doc.elementsByTagName("status").at(0).toElement().text();
		parseres.request_status = code.toInt();
		if (parseres.request_status != 200)
		{
			success = false;
			return;
		}
		QDomNodeList list = doc.elementsByTagName("richdata");
		if ( list.count() > 0)
		{
			parseres.queriesResult.push_back(list.at(0).toElement().text());
			parseres.one_position_entries_quantity = 1;
			success = true;
			return;
		}
		success = false;
	}
	catch (...)
	{
		success = false;
	}
}
