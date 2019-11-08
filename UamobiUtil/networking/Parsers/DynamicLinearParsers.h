#pragma once
#include "abs_parsed_request.h"
#include <QtXml/QDomDocument>

/*
	This class contains parsers, which one_position_entries_quantity is different
*/

class ModeListParser : public abs_parsed_request	//	parses mode lists
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;

	void parse_old_api(QDomDocument& ddoc);	//	old api has 2 fields
	void parse_new_api(QDomDocument& ddoc);	//	new api has one field
public:
	ModeListParser(QString& res, QString& err);
};
class ExpandedItemListParser : public abs_parsed_request
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;

	void parse_item_core(const QDomNode ddoc);
	void parse_item_optionals(const QDomNode ddoc);
public:
	ExpandedItemListParser(QString& res, QString& err);
};

class ItemInfoResponseParser : public abs_parsed_request
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	ItemInfoResponseParser(QString& res, QString& err);
};