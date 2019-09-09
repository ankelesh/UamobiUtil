#pragma once
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>
#include "networking/things.h"
#include "networking/Parsers/SimpleLinearParsers.h"
#include "networking/Parsers/SimpleResponseParsers.h"
#include "networking/Parsers/DynamicLinearParsers.h"
#include "networking/Parsers/NamedAttributesResponseParsers.h"
#include "legacy/legacy.h"
#include "debugtrace.h"


namespace parse_uniresults_functions
{
	enum reqtypes { None, login, login_R, modes, places , positional, simpliest};

	struct UserProfilesResult
	{
		QVector<UserProfile> profiles;
		bool manually;
	};
	struct TypicalResponce
	{
		QString errors;
		bool resp;
	};
	struct PositionalResponse
	{
		QHash<QString, QString> values;
		bool success;
	};
	typedef QVector<parsedMode> modesResponse;
	typedef QVector<parsedPlace> placesResponse;
	
	UserProfilesResult parse_user_profiles(uniform_parse_result& ures);
	modesResponse parse_modes(uniform_parse_result& ures);
	placesResponse parse_places(uniform_parse_result& ures);
	QHash<QString, QString> parse_positional_responses(uniform_parse_result& ures);

	bool isSimpliest(QString& res);
	bool isUserProfiles(QString & res);
	bool isLoginResponse(QString & res);
	bool isPositionalResponse(QString& res);
	bool isPlaceList(QString& res);
	bool isModeList(QString& res);
}

namespace RequestParser
{
	using namespace parse_uniresults_functions;
	reqtypes deduceRequestType(QString & res);
	abs_parsed_request* makeParser(QString& res, QString& errtext, reqtypes type = None);
	UserProfilesResult interpretAsLogin(QString& res, QString& errtext);
	TypicalResponce interpretAsLoginResponse(QString& res, QString& errtext);
	TypicalResponce interpretAsSimpliestResponse(QString& res, QString& errtext);
	modesResponse interpretAsModeList(QString& res, QString& errtext);
	placesResponse interpretAsPlaceList(QString& res, QString& errtext);
	PositionalResponse interpretAsPositionalResponse(QString& res, QString& errtext);
};


