#pragma once
#include <QSettings>
#include <QString>
#define LINELLA
#ifdef LINELLA
inline void initialize_default_file(QSettings& setting)
{
	if (!setting.allKeys().isEmpty())
		return;
	// settings default for all linella users
	setting.setValue("app_lang", "Russian");
	setting.setValue("http_host", "http://92.115.237.226:60080/um/unitest.php");
	setting.setValue("timeout", 240000);
	setting.setValue("scan_prefix", 36);
	setting.setValue("scan_suffix", 10);
	setting.setValue("font_min_height", 15);
	setting.setValue("font_max_height", 30);
	setting.setValue("font_scale_percent", 0.03);
	setting.setValue("printer_designation", "COM6:");
	setting.setValue("printer_port", 19200);
	setting.setValue("printer_type", "zebra");

}
#else
inline void initialize_default_file(QSettings& settings)
{

}
#endif