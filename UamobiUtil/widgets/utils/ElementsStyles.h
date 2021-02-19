#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#include <QtWidgets/qdesktopwidget.h>
#define GEOMETRY_SOURCE qApp->screens().first()
#else
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#define GEOMETRY_SOURCE qApp->desktop()
#endif
#include <QtGui/QScreen>
#include <QString>

#define GENERAL_FONT *FontAdapter::general()
#define AppFonts FontAdapter::instanse()

/*
	This file contains constants which are defining buttons stylesheets as a C-strings. all
	new styles must be added ONLY here to allow quick change if necessary.
	Naming: \state\_\element\_STYLESHEET

	Update:
		Now this file also stores additional inline functions for calculating adaptive sizes
		of interface elements. Their defaults are usually correspond most used values.
	Update:
		now theese functions are using actual screen size API instead of deprecated desktop()
*/

//These functions are used to calculate size in percent of all window


inline int calculateAdaptiveButtonHeight(double percent = 0.125)
//Calculates height for buttons, by default is giving 12.5% of screen height
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
    return int(GEOMETRY_SOURCE->availableGeometry().height() * percent);
}

inline int calculateAdaptiveWidth(double percent = 0.5)
//Calculates width, by default is giving 50% of screen width
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	return int(GEOMETRY_SOURCE->availableGeometry().height() * (percent * 0.66 * 0.66));
#endif
#endif
    return int(GEOMETRY_SOURCE->availableGeometry().width() * percent);
}
inline QSize calculateAdaptiveSize(double percent = 0.3)
//Calculates square size, by default is giving 30% of screen dimensions
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
	return QSize(
        int(GEOMETRY_SOURCE->availableGeometry().width() * percent),
        int(GEOMETRY_SOURCE->availableGeometry().height() * percent)
	);
}
inline QSize calculateAdaptiveSize(double Hpercent, double Wpercent)
//Calculates more adaptive size, allowing to scale both dimensions. No defaults.
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	Hpercent /= 2;
	Wpercent /= 4;
#endif
#endif
	return QSize(
        int(GEOMETRY_SOURCE->availableGeometry().width() * Wpercent),
        int(GEOMETRY_SOURCE->availableGeometry().height() * Hpercent)
	);
}

inline QSize imitatePhoneSize(double HPercent)
// sets window size in vertical projection using screen height
{
	return QSize(
        int(GEOMETRY_SOURCE->availableGeometry().height() * (HPercent * 0.66)),
        int(GEOMETRY_SOURCE->availableGeometry().height() * HPercent)
	);
}



class FontAdapter
	// creates scaled fonts
{
	int minheight;		//	minimum height of letter
	int maxheight;		//	maximum height of letter
	double minimumFontPercent;	// default font percent, which used to create original scaling
	int averageCharPerWidth;
	static FontAdapter* _instanse;
	static QFont* _generalFont;

	void _setCharPerWidth();
public:
	FontAdapter(int minheight, int maxheight, double minimumFontPercent);
	void reset(int mh, int Mh, double mfp);
	int howMuchCharacterFitsIntoScreen();

	static FontAdapter* instanse();
	static const QFont* general();
	// creates scaled font by using adapter. Extra percents are relational to calculated minheight
	static QFont makeFont(double extrapercents);
	static QFont makeCustomFont(double onlyPercents);
	QFont makeIndependentFont(int min, int max, int perc);
	static QString breakStringToFitScreen(QString src,const QFontMetrics& metrics, double percent = 1.0, int* breaks = Q_NULLPTR );
};

class StyleManager
{
	QStringList stockedStyles;
	StyleManager();
	static StyleManager* _instanse;
public:
	enum 
#ifdef QT_VERSION5X
		class
#endif	
		stylesheets
	{
		OK_BUTTON,
		BACK_BUTTON,
		COMMIT_BUTTON,
		DELETE_BUTTON,
		CHANGE_BUTTON,
		CHECKED_BUTTON,
		UP_SPINBOX_BUTTON,
		DOWN_SPINBOX_BUTTON,
		CANCEL_BUTTON,
		SETTINGS_BUTTON,
		NAVIGATE_BUTTON,
		BETTER_CALENDAR,
		ERROR_LABEL,
		UNCHECKED_BUTTON,
		CHECKBOX_BUTTON,
		LISTENING_CONTROL,
		FOCUSED_SPINBOX,
		FOCUSED_DATETIMEEDIT,
		FOCUSED_LINEEDIT,
		STYLESHEET_LIST_END
	};

	QColor EnumerablesFieldsColor;
	static const StyleManager* instanse();
	static const QString& getStyle(stylesheets);
	static void pushCurrentStylesheets();
};

QString& normalizeLine(QString& line);
QString normalizeLine(const QString line);


extern const QString & OK_BUTTONS_STYLESHEET;
// All buttons which are made for confirmation (commit button is separated from simple confirm)

extern const QString& BACK_BUTTONS_STYLESHEET;
// Back buttons must be same-styled through all application

extern const QString& COMMIT_BUTTONS_STYLESHEET;
// Commit is more saturated

extern const QString& DELETE_BUTTONS_STYLESHEET;
// Delete buttons must have this style and provide a warning messagebox

extern const QString& CHANGE_BUTTONS_STYLESHEET;
// All buttons which are allowing to change barcode must be same-styled so the user always knew it's functuon by color

extern const QString& CHECKED_BUTTONS_STYLESHEET;
// This style is specified to checked state of toggled buttons. use it for settings flags

extern const QString& UP_SPINBOX_STYLESHEET;
// up buttons of BigButtonsSpinbox

extern const QString& DOWN_SPINBOX_STYLESHEET;
// down buttons of BigButtonsSpinbox

extern const QString& CANCEL_BUTTONS_STYLESHEET;
// buttons that are quitting from complex branches must use this style

extern const QString& SETTINGS_BUTTONS_STYLESHEET;
// buttons which are opening settings

extern const QString& NAVIGATE_BUTTONS_STYLESHEET;
// Nav buttons differ with their enhanced disabled style

extern const QString& BETTER_CALENDAR_STYLESHEET;
// large calendar

extern const QString& ERROR_TEXT_STYLESHEET;
// red text for emergency messages

extern const QString& UNCHECKED_BUTTONS_STYLESHEET;
// stylesheet for toggled buttons

extern const QString& CHECKBOX_BUTTON_STYLESHEET;

extern const QString& LISTENING_CONTROL_STYLESHEET;

extern const QString& FOCUSED_SPINBOX_STYLESHEET;

extern const QString& FOCUSED_DATETIMEEDIT_STYLESHEET;

extern const QString& FOCUSED_LINEEDIT_STYLESHEET;
