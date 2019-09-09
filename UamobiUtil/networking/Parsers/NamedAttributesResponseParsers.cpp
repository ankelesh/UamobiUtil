#include "NamedAttributesResponseParsers.h"

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
#ifdef QT_VERSION5X
			auto li = doc.elementsByTagName("s");
#else
            QDomNodeList li = doc.elementsByTagName("s");
#endif
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
