#pragma once

#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>

extern const float VERSION;
extern const char * SUFFIX;

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
	QString printerPortDesignation;
	int printerPort;
	QString printerType;
	QStringList alternativePrinters;
	//  Global generated vars
	QTranslator translator;
	
	int fontMinHeight;
	int fontMaxHeight;
	double fontPercent;

	QString lastPrinterBTMAC;
	QString lastPrinterBTUUID;
	QString bluetoothDeviceNameMask;

    explicit GlobalAppSettings();
	
	void setTranslator();
	void dump();

	static GlobalAppSettings * instanse();
};
