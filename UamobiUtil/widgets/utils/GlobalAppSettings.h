#pragma once

#include "networking/dataupdateengine-http.h"
#include "submodules/UNAQtCommons/settings/CommonAppSettings.h"


extern const float VERSION;
extern const char * SUFFIX;

#define AppSettings GlobalAppSettings::instanse()

class GlobalAppSettings : public CommonAppSettings
{
protected:
	static GlobalAppSettings* _instanse;
	virtual p_QSettings _getSettingsFile() override;
	virtual void _save(p_QSettings) override;
	virtual void _load(p_QSettings) override;
public:
	//	Stored vars
	QString HttpUrl;
	QStringList AlternativeAdresses;
	QString printerPortDesignation;
	int printerPort;
	QString printerType;
	QStringList alternativePrinters;
	QStringList defaultDeviceNameMasks;

    explicit GlobalAppSettings();
	static GlobalAppSettings * instanse();
	static void init();
};
