#pragma once


#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>

const float VERSION = 0.0001;

struct GlobalAppSettings
{
	//	Stored vars	
	QString HttpUrl;
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
};
