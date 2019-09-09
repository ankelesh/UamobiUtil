#include "abs_parsed_request.h"

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

uniform_parse_result abs_parsed_request::read()
{
	return parseres;
}

bool queryLengthOkInResult(uniform_parse_result& ures)
{
	if (ures.queriesResult.count() == 0) {
		
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
