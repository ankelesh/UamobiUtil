#pragma once

#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>

const float VERSION = 0.003;
const char SUFFIX[] = "alpha";

class GlobalAppSettings
{
public:
	//	Stored vars
	QString HttpUrl;
	QStringList AlternativeAdresses;
	QString language;
	int timeoutInt;
	//  Global generated vars
	DataUpdateEngine* networkingEngine;
	QTranslator translator;

	// utility
	void setDefaultsToEmpty();
	void setTranslator();
	void dump();

	explicit GlobalAppSettings();
	~GlobalAppSettings();
};
