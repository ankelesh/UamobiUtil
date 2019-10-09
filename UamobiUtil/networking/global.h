#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSettings>
#ifdef GUI_APP
#include <QApplication>
#include <QWidget>
#else
#include <QtCore/QCoreApplication>
#endif
const char* const SrvUnderstandableDateFormat = "dd.mm.yyyy";

inline QSettings* getSettingsObject()
{
	static QSettings s(qApp->applicationDirPath() + "/settings.ini", QSettings::IniFormat);
	s.sync();
	return &s;
}

inline QString bool2str(bool b)
{
	if (b)
		return "true";
	else
		return "false";
}

#ifdef GUI_APP
void reloadTranslation();
inline void setWidgetMobileFlags(QWidget* w)
{
#ifdef Q_OS_WINCE
	if (w->window() == w)
		w->setWindowFlags((w->windowFlags() | Qt::FramelessWindowHint | Qt::CustomizeWindowHint)
			& ~(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint
				| Qt::WindowCloseButtonHint | Qt::Dialog | Qt::WindowContextHelpButtonHint));
#else
	Q_UNUSED(w);
#endif
}

inline void showWidget(QWidget* w)
{
	w->showNormal();
#ifdef Q_OS_WINCE
	w->showMaximized(); // stupid windows management... showNormal() is required, yes
#endif // Q_OS_WINCE
}

int intShKey(QObject* osh);
#endif
#endif // GLOBAL_H
