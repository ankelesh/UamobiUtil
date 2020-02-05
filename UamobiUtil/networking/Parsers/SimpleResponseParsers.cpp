#include "SimpleResponseParsers.h"
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

SimpliestResponceParser::SimpliestResponceParser(QString& res, QString& err)
	: AbsResponseParser(res, err)
{
	success = run();
}

bool SimpliestResponceParser::_doParsing()
{
	return !isError(xmldoc);
}

bool RichtextResponseParser::_doParsing()
{
	QDomNodeList tempList = xmldoc.childNodes();
	if (tempList.count() < 2)
		return _stopWithError("No richtext object detected");
	QDomNode listNode = tempList.at(1);
	parseResult << XmlObject(new UniformXmlObject(UniformXmlObject::Richtext, listNode));
	if (!parseResult.first()->hasField("richdata"))
	{
		errtext += " No richdata field found in main object!";
		return false;
	}
	return true;
}

RichtextResponseParser::RichtextResponseParser(QString& res, QString& err)
	: AbsResponseParser(res,err)
{
	success = run();
}
