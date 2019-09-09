#pragma once
#include "networking/Parsers/abs_parsed_request.h"
#include <QtXml/QDomDocument>

/*
	These parsers are made to work over simpliest responces, which are only holding success\insuccess value.
*/

class SimpliestResponceParser : public  abs_parsed_request // This parser only checks for 200 response
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	SimpliestResponceParser(QString& res, QString& err);
};
typedef SimpliestResponceParser LoginResponseParser;		//	This typedef exists because LoginResponseParser was implemented before SRP