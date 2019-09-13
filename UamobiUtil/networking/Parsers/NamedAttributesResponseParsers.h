#pragma once
#include "abs_parsed_request.h"
#include <QtXml/QDomDocument>

/*
	This file contains parsers which must provide named list of parsed args. Normal order is
	 Name : value and typical one_entry_position_quantity =2
*/

class ModeSelectResponseParser : public abs_parsed_request	//	parses mode_select response which usually returns settings values
{
protected:

	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	ModeSelectResponseParser(QString& res, QString& err);
};
class DocumentCreationResponseParser : public abs_parsed_request
{
protected:

	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	DocumentCreationResponseParser(QString& res, QString& err);
};