#include "SimpleLinearParsers.h"

bool UserListParser::couldRead()
{
	return success;
}

bool UserListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString UserListParser::parseErrorText()
{
	return errtext;
}

UserListParser::UserListParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
	detrace_METHDATAS("UserListParser::dconstr", "code", <<code)
	if (parseres.request_status != 200)
		return;
	if (doc.elementsByTagName("manually").count() > 0)
	{
		detrace_METHEXPL("manually found in the request!")
		parseres.alternative_result = 1;
	}
	else
	{
		parseres.alternative_result = 0;
	}
	QDomNodeList dmndl = doc.elementsByTagName("user");
	for (int i = 0; i < dmndl.count(); ++i)
	{
		parseres.queriesResult << (dmndl.at(i).childNodes().at(1).toElement().text());
		parseres.queriesResult << (dmndl.at(i).childNodes().at(0).toElement().text());
	}
	
	
	parseres.one_position_entries_quantity = 2;
	parseres.type = linear_result;
	success = true;
}

bool PlacesListParser::couldRead()
{
	return success;
}

bool PlacesListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString PlacesListParser::parseErrorText()
{
	return errtext;
}

PlacesListParser::PlacesListParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code)
	if (parseres.request_status != 200)
		return;
	QDomNodeList dmndl = doc.elementsByTagName("place");
	for (int i = 0; i < dmndl.count(); ++i)
	{
		parseres.queriesResult <<  dmndl.at(i).childNodes().at(0).toElement().text();
		parseres.queriesResult <<  dmndl.at(i).childNodes().at(1).toElement().text();
	}
	parseres.type = linear_result;
	parseres.one_position_entries_quantity = 2;
	success = true;
}

bool SuppliersListParser::couldRead()
{
	return success;
}

bool SuppliersListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString SuppliersListParser::parseErrorText()
{
	return errtext;
}

SuppliersListParser::SuppliersListParser(QString& res, QString& errtext)
	: abs_parsed_request(res, errtext)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code)
	if (parseres.request_status != 200)
		return;
	QDomNodeList dmndl = doc.elementsByTagName("supplier");
	for (int i = 0; i < dmndl.count(); ++i)
	{
		parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
		parseres.queriesResult << dmndl.at(i).childNodes().at(1).toElement().text();
		parseres.queriesResult << dmndl.at(i).childNodes().at(2).toElement().text();
	}
	parseres.type = linear_result;
	parseres.one_position_entries_quantity = 3;
	success = true;
}

bool OrdersListParser::couldRead()
{
	return success;
}

bool OrdersListParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString OrdersListParser::parseErrorText()
{
	return errtext;
}

OrdersListParser::OrdersListParser(QString& res, QString& errtext)
	: abs_parsed_request(res, errtext)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code)
	if (parseres.request_status != 200)
		return;
	QDomNodeList dmndl = doc.elementsByTagName("order");
	for (int i = 0; i < dmndl.count(); ++i)
	{
		parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
		parseres.queriesResult << dmndl.at(i).childNodes().at(1).toElement().text();
		parseres.queriesResult << dmndl.at(i).childNodes().at(2).toElement().text();
	}
	parseres.type = linear_result;
	parseres.one_position_entries_quantity = 3;
	success = true;
}
