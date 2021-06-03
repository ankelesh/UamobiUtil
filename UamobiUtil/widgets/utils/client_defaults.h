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
	setting.beginGroup("common");
	setting.setValue("scan_prefix", 36);
	setting.setValue("scan_suffix", 10);
	setting.setValue("font_min_height", 15);
	setting.setValue("font_max_height", 30);
	setting.setValue("font_scale_percent", 0.03);
	setting.setValue("app_lang", "Russian");
	setting.endGroup();
	setting.beginGroup("network");
	setting.setValue("http_host", "http://192.168.0.45/um/uniback_2020.php");
	setting.setValue("timeout", 240000);
	setting.endGroup();
	setting.beginGroup("label_printer");
	setting.setValue("printer_designation", "COM6:");
	setting.setValue("printer_port", 19200);
	setting.setValue("printer_type", "zebra20_70x33");
	QStringList t;
	t << "tsc20_70x33";
	setting.setValue("printer_alternatives", t);
	setting.endGroup();

}
#else
inline void initialize_default_file(QSettings& settings)
{

}
#endif