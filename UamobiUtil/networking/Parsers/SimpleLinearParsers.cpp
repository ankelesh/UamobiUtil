#include "SimpleLinearParsers.h"
#include <QApplication>


bool LinearListParser::_doParsing()
{
	QDomNodeList tempList = xmldoc.childNodes();
	if (tempList.count() < 2)
		return _stopWithError("No list detected");
	QDomNode listNode = tempList.at(1);
	QDomNodeList listedObjects = listNode.childNodes();
	if (listedObjects.isEmpty())
		return _stopWithError("List unpacking fail: no status code");
	
	assertAndSwitchToNewStyle(listNode, listedObjects);
	
	for (int i = 1; i < listedObjects.count(); ++i)
	{
		parseResult << XmlObject(new UniformXmlObject(listedObjects.at(i)));
	}
	return true;
}

void LinearListParser::assertAndSwitchToNewStyle(QDomNode& node, QDomNodeList& list)
{
	if (list.at(0).nodeName() == "announcement")
	{
		node = list.at(1);
		list= node.childNodes();
	}
}

XmlObjects LinearListParser::parseLinearList(QString& r, QString& e)
{
	return LinearListParser(r,e).read();
}

bool LinearListWithSublistParser::_doParsing()
{
	if (!errtext.isEmpty())
		return false;
	QDomNodeList tempList = xmldoc.childNodes();
	if (tempList.count() < 2)
		return _stopWithError("No list detected");
	QDomNode listNode = tempList.at(1);
	QDomNodeList listedObjects = listNode.childNodes();
	if (listedObjects.isEmpty())
		return _stopWithError("List unpacking fail: no status code");
	XmlObject statusStringCollector(new UniformXmlObject(UniformXmlObject::StatusString));
	for (int i = 0; i < listedObjects.count(); ++i)
	{
		if (listedObjects.at(i).firstChild().nodeType() != QDomNode::ElementNode)
		{
			statusStringCollector->addField(
				listedObjects.at(i).nodeName(), listedObjects.at(i).toElement().text());
		}
		else
		{
			tempList = listedObjects.at(i).childNodes();
			for (int j = 0; j < tempList.count(); ++j)
			{
				if (tempList.at(j).firstChild().nodeType() != QDomNode::ElementNode)
				{
					_stopWithError("double folded value located");
				}
				else
				{
					parseResult << XmlObject(new UniformXmlObject(tempList.at(j)));
				}
			}
		}
	}
	if (statusStringCollector->mySize() > 0)
		parseResult << statusStringCollector;
	return true;
}
