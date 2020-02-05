#include "GlobalAppSettings.h"
#include <QtCore/QSettings>
#include <QApplication>
// utility


void GlobalAppSettings::setTranslator()
{
	if (this->language == "Russian")
	{
		this->translator.load(":/translations/uamobi_ru.qm");
	}
	else if (this->language == "Romanian")
	{
		this->translator.load(":/translations/uamobi_ro.qm");
	}
	else
	{
		this->translator.load(":/translations/uamobi_en.qm");
	}
	qApp->installTranslator(&translator);
}

void GlobalAppSettings::dump()
{
	QSettings setting("settings.ini", QSettings::IniFormat);
	setting.setValue("app_lang", language);
	setting.setValue("http_host", HttpUrl);
	setting.setValue("timeout", timeoutInt);
	setting.setValue("alt_hosts", AlternativeAdresses);
	setting.setValue("scan_prefix", scanPrefix);
	setting.setValue("scan_suffix", scanSuffix);
}

GlobalAppSettings* GlobalAppSettings::_instanse = Q_NULLPTR;
GlobalAppSettings* GlobalAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
	}
	return _instanse;
}
GlobalAppSettings::GlobalAppSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	HttpUrl = settings.value("http_host", "http://una.md:3323/").toString();
	language = settings.value("app_lang", "English").toString();
	timeoutInt = settings.value("timeout", 200000000).toInt();
	AlternativeAdresses = settings.value("alt_hosts", QVariant(QStringList())).toStringList();
	scanPrefix = settings.value("scan_prefix", 36).toInt();
	scanSuffix = settings.value("scan_suffix", 10).toInt();
	setTranslator();
}
