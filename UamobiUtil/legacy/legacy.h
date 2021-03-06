#pragma once
#include "networking/things.h"
#include <QtCore/QString>
#include <QtCore/QHash>

/*
	This file contains any shitty solution which was derived from old project and now is required to save
	time or because there is no alternative due to web api architecture. If solution is fixed and now can be included into
	main project - it can be moved out of here.

		parseLegacyMode - large elif sequence to determine mode and submode recognized by single string
		filterNonCompatibleModes - wipes from vector of modes all non-supported ones
*/
namespace legacy {
	QPair<QString, QString> splitLegacyMode( QString& mode);
	int guessControlType(QString& cname);		//	guesses type of control from it's name
}
