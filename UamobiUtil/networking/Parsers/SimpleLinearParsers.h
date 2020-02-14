#pragma once
#include "abs_parsed_request.h"
#include <QtXml/QDomDocument>
#include "debugtrace.h"

/*
	This file contains all requests that does not have any named components and can be represented as long list of
	same size sections. Section size is stored in one_position_entries_quantity. These parsers are using full list without
	empty spaces and one_position_entries_quantity is stable - one parser class provides same value in any modification

*/


class LinearListParser : public AbsResponseParser
{
protected:
	virtual bool _doParsing() override;
	// switch to new style serialization if possible
	UniformXmlObject::ThingsIds assertAndSwitchToNewStyle(QDomNode& node, QDomNodeList& list);
public:
	LinearListParser(QString res, QString err) : AbsResponseParser(res, err) { success = run(); };
	static XmlObjects parseLinearList(QString & r, QString& e);
};

class LinearListWithSublistParser : public AbsResponseParser
{
protected:
	// Inherited via AbsResponseParser
	virtual bool _doParsing() override;
public:
	LinearListWithSublistParser(QString res, QString err) : AbsResponseParser(res, err) { success = run(); };

};



