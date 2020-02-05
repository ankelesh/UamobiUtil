#pragma once

#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>

const float VERSION = 0.01;
const char SUFFIX[] = "nightly";

#define AppSettings GlobalAppSettings::instanse()

class GlobalAppSettings
{
protected:
	static GlobalAppSettings* _instanse;
public:
	//	Stored vars
	QString HttpUrl;
	QStringList AlternativeAdresses;
	QString language;
	int timeoutInt;
	int scanPrefix;
	int scanSuffix;
	//  Global generated vars
	QTranslator translator;

	explicit GlobalAppSettings();
	
	void setTranslator();
	void dump();

	static GlobalAppSettings * instanse();
};
