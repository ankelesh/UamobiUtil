#pragma once
#include "abs_parsed_request.h"
#include <QtXml/QDomDocument>
#include "debugtrace.h"

/*
	This file contains all requests that does not have any named components and can be represented as long list of
	same size sections. Section size is stored in one_position_entries_quantity. These parsers are using full list without
	empty spaces and one_position_entries_quantity is stable - one parser class provides same value in any modification

*/
class UserListParser : public abs_parsed_request // parses lists of users
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	UserListParser(QString& res, QString& err);
};

class PlacesListParser : public abs_parsed_request // parses lists of places
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	PlacesListParser(QString& res, QString& err);
};
class SuppliersListParser : public abs_parsed_request
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;

	virtual bool noRequestErrors() override;

	virtual QString parseErrorText() override;
public:
	SuppliersListParser(QString& res, QString& errtext);
};
class OrdersListParser : public abs_parsed_request
{
protected:

	// Inherited via abs_parsed_request
	virtual bool couldRead() override;

	virtual bool noRequestErrors() override;

	virtual QString parseErrorText() override;
public:
	OrdersListParser(QString& res, QString& errtext);
};
class ItemSimplifiedParser : public abs_parsed_request
{
protected:


	// Inherited via abs_parsed_request
	virtual bool couldRead() override;

	virtual bool noRequestErrors() override;

	virtual QString parseErrorText() override;

public:
	ItemSimplifiedParser(QString& res, QString& errtext);
};
class DocumentParser : public abs_parsed_request
{
protected:

	// Inherited via abs_parsed_request
	virtual bool couldRead() override;
	virtual bool noRequestErrors() override;
	virtual QString parseErrorText() override;
public:
	DocumentParser(QString& res, QString& err);
};
class DocTypeFiltersParser : public abs_parsed_request
{
protected:
	// Inherited via abs_parsed_request
	virtual bool couldRead() override;

	virtual bool noRequestErrors() override;

	virtual QString parseErrorText() override;
public:
	DocTypeFiltersParser(QString& res, QString& err);
};