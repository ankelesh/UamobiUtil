#include "GlobalAppSettings.h"
#include <QtCore/QSettings>
#include <QApplication>
// utility
#include "client_defaults.h"

const float VERSION = 0.14f;
#ifdef LINELLA
const char* SUFFIX = "lin-alpha";
#else
const char* SUFFIX = "alpha";
#endif
void GlobalAppSettings::setTranslator()
{
	if (this->language == "Russian")
	{
		this->translator.load(":/translations/uamobiutil_ru.qm");
	}
	else if (this->language == "Romanian")
	{
		this->translator.load(":/translations/uamobiutil_ro.qm");
	}
	else
	{
		this->translator.load(":/translations/uamobiutil_en.qm");
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
	setting.setValue("font_min_height", fontMinHeight);
	setting.setValue("font_max_height", fontMaxHeight);
	setting.setValue("font_scale_percent", fontPercent);
    setting.setValue("printer_designation", printerPortDesignation);
    setting.setValue("printer_port", printerPort);
    setting.setValue("printer_type", printerType);
	setting.setValue("printer_alternatives", alternativePrinters);
    setting.setValue("notifications_volume", notificationsVolume);
	setting.beginGroup("bluetooth");
	setting.setValue("lastMAC", lastPrinterBTMAC);
	setting.setValue("lastUUID", lastPrinterBTUUID);
	setting.setValue("deviceNameMask", bluetoothDeviceNameMask);
	setting.setValue("defaultDeviceNameMasks", defaultDeviceNameMasks);
	setting.endGroup();
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
	initialize_default_file(settings);
	HttpUrl = settings.value("http_host", "http://una.md:3323/").toString();
	language = settings.value("app_lang", "English").toString();
	timeoutInt = settings.value("timeout", 20000).toInt();
	AlternativeAdresses = settings.value("alt_hosts", QVariant(QStringList())).toStringList();
	scanPrefix = settings.value("scan_prefix", 36).toInt();
	scanSuffix = settings.value("scan_suffix", 10).toInt();
	fontMinHeight = settings.value("font_min_height", 15).toInt();
	fontMaxHeight = settings.value("font_max_height", 30).toInt();
	fontPercent = settings.value("font_scale_percent", 0.03).toDouble();
    printerPortDesignation = settings.value("printer_designation", "COM6:").toString();
	printerPort = settings.value("printer_port", 19200).toInt();
	printerType = settings.value("printer_type", "zebra").toString();
	alternativePrinters = settings.value("alternative_printers", QVariant(QStringList())).toStringList();
    notificationsVolume = settings.value("notifications_volume", QVariant(0.75)).toDouble();
	settings.beginGroup("bluetooth");
	lastPrinterBTMAC = settings.value("lastMAC", QString()).toString();
	lastPrinterBTUUID = settings.value("lastUUID", QString()).toString();
	bluetoothDeviceNameMask = settings.value("deviceNameMask", QString("zebra")).toString();
	defaultDeviceNameMasks = settings.value("defaultDeviceNameMasks", QStringList()).toStringList();
	settings.endGroup();
	setTranslator();

}
