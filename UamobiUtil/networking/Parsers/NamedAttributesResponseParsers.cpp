#include "NamedAttributesResponseParsers.h"

#include "debugtrace.h"

bool ModeSelectResponseParser::couldRead()
{
	return success;
}

bool ModeSelectResponseParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString ModeSelectResponseParser::parseErrorText()
{
	return errtext;
}

ModeSelectResponseParser::ModeSelectResponseParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	try
	{
		QString code = doc.elementsByTagName("status").at(0).toElement().text();
		parseres.request_status = code.toInt();
		if (parseres.request_status != 200)
		{
#ifdef QT_VERSION5X
			auto li = doc.elementsByTagName("message");
#else
			QDomNodeList li = doc.elementsByTagName("message");
#endif
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
		else
		{
			QDomNodeList li = doc.elementsByTagName("s");
			if (li.count() == 0)
			{
				success = false;
				return;
			}
			for (int i = 0; i < li.count(); ++i)
			{
				parseres.queriesResult << li.at(0).toElement().text();
				parseres.queriesResult << li.at(1).toElement().text();
			}
			parseres.one_position_entries_quantity = 2;
			parseres.type = request_parse_type::positional_result;
		}
		success = true;
	}
	catch (...)
	{
		success = false;
	}
}

bool DocumentCreationResponseParser::couldRead()
{
	return success;
}

bool DocumentCreationResponseParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString DocumentCreationResponseParser::parseErrorText()
{
	return errtext;
}

DocumentCreationResponseParser::DocumentCreationResponseParser(QString& res, QString& err)
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
		else
		{
			QDomNodeList li = doc.elementsByTagName("doc");
			if (li.count() == 0)
			{
				success = false;
				return;
			}
			QDomNodeList parms = doc.childNodes();
			for (int i = 0; i < parms.count(); ++i)
			{
				parseres.queriesResult << parms.at(i).nodeName();
				parseres.queriesResult << li.at(i).toElement().text();
			}
			parseres.one_position_entries_quantity = 2;
			parseres.type = request_parse_type::positional_result;
		}
		success = true;
	}
	catch (...)
	{
		success = false;
	}
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
	: abs_parsed_request(res,err)
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
		parseres.queriesResult << "nn" << dmndl.at(0).toElement().text();
	}
	dmndl = doc.elementsByTagName("control");
	QDomNode cntrl;
	for (int i = 0; i < dmndl.count(); ++i)
	{
		cntrl = dmndl.at(i);
		detrace_METHEXPL("got control");
		QDomNodeList insides = cntrl.childNodes();
		for (int j = 0; j < insides.count(); ++j)
		{
			detrace_METHEXPL("getting inside: " << insides.at(j).toElement().text());
			parseres.queriesResult << insides.at(j).toElement().text();
		}
	}
	dmndl = doc.elementsByTagName("richdata");
	if (dmndl.count() > 0)
	{
		parseres.queriesResult << "richdata" << dmndl.at(0).toElement().text();
	}
	else
	{
		parseres.queriesResult << "richdata" << "";
	}
	success = true;
	parseres.one_position_entries_quantity = 2;
}
