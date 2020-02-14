#include "abs_parsed_request.h"
#include <QtCore/QString>
#include <QtCore/QDataStream>


QString makeError(QDomDocument& xmldoc)
{
	QDomNodeList tlist = xmldoc.elementsByTagName("status");
	QString errtext;
	if (tlist.count() < 1)
	{
		return "No status code";
	}
	else
	{
		int code = tlist.at(0).toElement().text().toInt();
		switch (code)
		{
		case 200:
			errtext = "possibly fake error, check logs";
			break;
		case 403:
			errtext = "Forbidden request";
			break;
		case 404:
			errtext = "Data not exists";
			break;
		case 500:
			errtext = "Internal error";
			break;
		default:
			errtext = "Unknown error code: "
				+ tlist.at(0).toElement().text();
			break;
		}
	}
	tlist = xmldoc.elementsByTagName("message");
	if (tlist.count() > 0)
		errtext += tlist.at(0).toElement().text();
	return errtext;
}
bool isError(QDomDocument& xmldoc)
{
	QDomNodeList tlist = xmldoc.elementsByTagName("status");
	if (tlist.count() < 1)
	{
		return true;
	}
	if (tlist.at(0).toElement().text().toInt() != 200)
	{
		return true;
	}
	return false;
}

bool AbsResponseParser::_stopWithError(const char* estr)
{
	xmldoc.clear();
	errtext += estr;
	return false;
}

bool AbsResponseParser::run()
{
	return _doParsing();
}

AbsResponseParser::AbsResponseParser(QString& res, QString& err)
	: xmldoc(), errtext(err), success(false), parseResult(), alternativeResult(0)
{
	if (res.isEmpty())
	{
		errtext = "Empty result!";
		return;
	}
	xmldoc.setContent(res);
	if (isError(xmldoc))
	{
		errtext = makeError(xmldoc);
	}	
}

bool AbsResponseParser::isSuccessfull()
{
	return success;
}

QString AbsResponseParser::getErrors()
{
	return errtext;
}

int AbsResponseParser::isAlternative()
{
	return alternativeResult;
}

void AbsResponseParser::reset(QString& res, QString& err)
{
	xmldoc.setContent(res);
	errtext = err;
	if (isError(xmldoc))
	{
		errtext += makeError(xmldoc);
	}
	else
	{
		success = _doParsing();
	}
}

XmlObjects& AbsResponseParser::read()
{
	return parseResult;
}
