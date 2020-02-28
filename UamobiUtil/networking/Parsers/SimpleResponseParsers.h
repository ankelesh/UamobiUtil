#pragma once
#include "networking/Parsers/abs_parsed_request.h"
#include <QtXml/QDomDocument>
/*
	These parsers are made to work over simpliest responces, which are only holding success\insuccess value.
*/

class SimpliestResponceParser : public  AbsResponseParser // This parser only checks for 200 response
{
protected:
	// Inherited via AbsResponseParser
	virtual bool _doParsing() override;
public:
	SimpliestResponceParser(QString& res, QString& err);

};
class RichtextResponseParser : public AbsResponseParser
{
protected:
	virtual bool _doParsing() override;
public:
	RichtextResponseParser(QString& res, QString& err);
	QString getRichtext();
};
