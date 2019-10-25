#include "GlobalAppSettings.h"
#include <QtCore/QSettings>

// utility

void GlobalAppSettings::setDefaultsToEmpty()
{
	if (HttpUrl.isEmpty()) { HttpUrl = "una.md:3323/um/sammy_clouddev.php"; }
	if (language.isEmpty()) { language = "English"; }
}

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
}

GlobalAppSettings::GlobalAppSettings()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	HttpUrl = settings.value("http_host", "").toString();
	language = settings.value("app_lang", "").toString();
	timeoutInt = settings.value("timeout", 200000000).toInt();
	AlternativeAdresses = settings.value("alt_hosts").toStringList();
	setDefaultsToEmpty();
	setTranslator();
}

GlobalAppSettings::~GlobalAppSettings()
{
	dump();
}
