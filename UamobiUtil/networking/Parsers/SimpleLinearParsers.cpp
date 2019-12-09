#include "SimpleLinearParsers.h"
#include <QApplication>
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
#ifdef DEBUG
	//detrace_METHTEXTS("UserListParser", "res", res);
#endif
	QDomDocument doc;
	doc.setContent(result);
#ifdef DEBUG
	//detrace_METHEXPL("errors: " << doc.toString());
#endif
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", <<code);
#endif
	if (parseres.request_status != 200)
		return;
	if (doc.elementsByTagName("manually").count() > 0)
	{
#ifdef DEBUG
		//detrace_METHEXPL("manually found in the request!");
#endif
		parseres.alternative_result = 1;
	}
	else
	{
		parseres.alternative_result = 0;
	}
	QDomNodeList dmndl = doc.elementsByTagName("user");
#ifdef DEBUG
	//detrace_METHEXPL("dmndl len: " << dmndl.count());
#endif
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
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	if (parseres.request_status != 200)
		return;
	QDomNodeList dmndl = doc.elementsByTagName("place");
    int j = 0;
	for (int i = 0; i < dmndl.count(); ++i)
	{
		parseres.queriesResult << dmndl.at(i).childNodes().at(0).toElement().text();
		parseres.queriesResult << dmndl.at(i).childNodes().at(1).toElement().text();
        if ( i - j > 20)
        {
            qApp->processEvents();
            j+=20;
        }
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
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
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
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
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

bool ItemSimplifiedParser::couldRead()
{
	return success;
}

bool ItemSimplifiedParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString ItemSimplifiedParser::parseErrorText()
{
	return errtext;
}

ItemSimplifiedParser::ItemSimplifiedParser(QString& res, QString& errtext)
	: abs_parsed_request(res, errtext)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	if (parseres.request_status != 200)
		return;
	QDomNodeList pageinfo = doc.elementsByTagName("page");
	if (pageinfo.isEmpty())
	{
		success = false;
		return;
	}
	parseres.queriesResult << pageinfo.at(0).namedItem("from").toElement().text();
	parseres.queriesResult << pageinfo.at(0).namedItem("to").toElement().text();
	parseres.queriesResult << pageinfo.at(0).namedItem("last").toElement().text();
	parseres.alternative_result = 3;
	QDomNodeList dmndl = doc.elementsByTagName("result");
	int len = dmndl.count();
	for (int i = 0; i < len; ++i)
	{
		parseres.queriesResult << dmndl.at(i).namedItem("code").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("title").toElement().text();
	}
	parseres.one_position_entries_quantity = 2;
	success = true;
}

bool DocumentParser::couldRead()
{
	return success;
}

bool DocumentParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString DocumentParser::parseErrorText()
{
	return errtext;
}

DocumentParser::DocumentParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	QDomNodeList dmndl = doc.elementsByTagName("parentdoc");
	int len = dmndl.count();
	for (int i = 0; i < len; ++i)
	{
		parseres.queriesResult << dmndl.at(i).namedItem("cod").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("title").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("text").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("doctype").toElement().text();
	}
	parseres.one_position_entries_quantity = 4;
	success = true;
}

bool DocTypeFiltersParser::couldRead()
{
	return success;
}

bool DocTypeFiltersParser::noRequestErrors()
{
	return parseres.request_status == 200;
}

QString DocTypeFiltersParser::parseErrorText()
{
	return errtext;
}

DocTypeFiltersParser::DocTypeFiltersParser(QString& res, QString& err)
	: abs_parsed_request(res, err)
{
	QDomDocument doc;
	doc.setContent(result);
	QString code = doc.elementsByTagName("status").at(0).toElement().text();
	parseres.request_status = code.toInt();
#ifdef DEBUG
	//detrace_METHDATAS("UserListParser::dconstr", "code", << code);
#endif
	QDomNodeList dmndl = doc.elementsByTagName("type");
	int len = dmndl.count();
	for (int i = 0; i < len; ++i)
	{
		parseres.queriesResult << dmndl.at(i).namedItem("id").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("name").toElement().text();
		parseres.queriesResult << dmndl.at(i).namedItem("filtered-out").toElement().text();
	}
	parseres.one_position_entries_quantity = 3;
	success = true;
}
