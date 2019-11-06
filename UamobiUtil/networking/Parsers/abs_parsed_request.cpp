#include "abs_parsed_request.h"
#include <QtCore/QString>
#include <QtCore/QDataStream>

abs_parsed_request::abs_parsed_request(QString& res, QString& err)
	: result(res), errtext(err), success(false)
{
}

bool abs_parsed_request::isSuccessfull()
{
	return success;
}

QString abs_parsed_request::getErrors()
{
	if (!couldRead())
		return QString("Error reading request");
	if (!noRequestErrors())
		return parseErrorText();
	else
		return QString("Other error");
}

uniform_parse_result& abs_parsed_request::read()
{
	return parseres;
}

bool queryLengthOkInResult(uniform_parse_result& ures)
{
	if (ures.queriesResult.count() == 0 || ures.one_position_entries_quantity == 0) {
		return false;
	}
	return (ures.queriesResult.count() % ures.one_position_entries_quantity) == 0;
}

int queryReservationSize(uniform_parse_result& ures)
{
	if (ures.queriesResult.count() == 0)
		return 1;
	return ures.queriesResult.count() / ures.one_position_entries_quantity;
}

QString showHeap(uniform_parse_result& ures)
{
	QString temp("heap snapshot with tech values:");
	temp += QString::number(ures.one_position_entries_quantity) + " opeq, " + QString::number(ures.alternative_result) + " ares,";
	temp += QString::number(ures.request_status) + " status\r\n";
	for (int i = 0; i < ures.queriesResult.count(); ++i)
	{
		temp += "\r\n|" + ures.queriesResult.at(i) + "|<" + QString::number(i) + ">";
	}

	return temp;
}

uniform_parse_result::uniform_parse_result(request_parse_type t)
	: type(t), queriesResult(), request_status(0), one_position_entries_quantity(0), alternative_result(0)
{
}