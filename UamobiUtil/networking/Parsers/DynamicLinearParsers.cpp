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

bool TwoStateListParser::_doParsing()
{
	QDomNodeList tempList = xmldoc.childNodes();
	if (tempList.count() < 2)
		return _stopWithError("No list detected");
	QDomNode listNode = tempList.at(1);
	QDomNodeList listedObjects = listNode.childNodes();
	if (listedObjects.isEmpty())
		return _stopWithError("List unpacking fail: no status code");
	QDomNode subnode;
	for (int i = 1; i < listedObjects.count()-1; ++i)
	{
		subnode = listedObjects.at(i);
		tempList = subnode.childNodes();
		if (tempList.count() == 1)
		{
			parseResult << XmlObject(new UniformXmlObject(listNode));
			alternativeResult = 1;
			return true;
		}
		else
		{
			parseResult << XmlObject(new UniformXmlObject(subnode));
		}
	}
	subnode = listedObjects.at(listedObjects.count() - 1);
	parseResult.push_back(XmlObject(new UniformXmlObject(UniformXmlObject::StatusString, subnode.nodeName(), subnode.toElement().text())));
	return true;
}

