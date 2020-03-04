#pragma once
#include "abs_parsed_request.h"
#include <QtXml/QDomDocument>

/*
	This class contains parsers, which are holding non homogenic results
*/


class TwoStateListParser : public AbsResponseParser
{
protected:
	virtual bool _doParsing() override;
public:
    TwoStateListParser(QString res, QString err) : AbsResponseParser(res, err) { success = run(); }
};
