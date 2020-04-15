#include "ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif
#include <qsettings.h>
#include <cmath>
/*
	This file contains stylesheet definitions
*/
QString& normalizeLine(QString& line)
{
#ifdef Q_OS_WINCE
    int inserts = line.length() / 22;
    if (inserts <= 0)
        return line;
    for (int i = 1; i <= inserts; ++i)
    {
        line.insert(i * 24 + i - 1, "\n");
    }
#endif
    return line;
}
QString normalizeLine(const QString line)
{
    QString nline = line;
#ifdef Q_OS_WINCE
    int inserts = nline.length() / 22;
    if (inserts <= 0)
        return nline;
    for (int i = 1; i <= inserts; ++i)
    {
        nline.insert(i * 24 + i - 1, "\n");
    }
#endif
    return nline;
}
const QString& OK_BUTTONS_STYLESHEET = 
StyleManager::getStyle(StyleManager::stylesheets::OK_BUTTON);

const QString& BACK_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::BACK_BUTTON);

const QString& COMMIT_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::COMMIT_BUTTON);

const QString &DELETE_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::DELETE_BUTTON);

const QString& CHANGE_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::CHANGE_BUTTON);

const QString& CHECKED_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::CHECKED_BUTTON);

const QString& UP_SPINBOX_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::UP_SPINBOX_BUTTON);

const QString& DOWN_SPINBOX_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::DOWN_SPINBOX_BUTTON);

const QString& BETTER_CALENDAR_STYLESHEET = 
StyleManager::getStyle(StyleManager::stylesheets::BETTER_CALENDAR);
const QString& ERROR_TEXT_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::ERROR_LABEL);

const QString& CANCEL_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::CANCEL_BUTTON);
;

const QString &SETTINGS_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::SETTINGS_BUTTON);

const QString& UNCHECKED_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::UNCHECKED_BUTTON);

const QString& NAVIGATE_BUTTONS_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::NAVIGATE_BUTTON);

const QString& LISTENING_CONTROL_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::LISTENING_CONTROL);

const QString& CHECKBOX_BUTTON_STYLESHEET = 
StyleManager::getStyle(StyleManager::stylesheets::CHECKBOX_BUTTON);

const QString& FOCUSED_SPINBOX_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::FOCUSED_SPINBOX);

const QString& FOCUSED_DATETIMEEDIT_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::FOCUSED_DATETIMEEDIT);

const QString& FOCUSED_LINEEDIT_STYLESHEET =
StyleManager::getStyle(StyleManager::stylesheets::FOCUSED_LINEEDIT);

const QFont & makeFont(double /*perc*/)
{
	return *(FontAdapter::general());
}

void FontAdapter::_setCharPerWidth()
{
	QFontMetrics fm(*general());
	int width = calculateAdaptiveWidth(0.95);
	if (fm.averageCharWidth() == 0 || width == 0)
		averageCharPerWidth = 10;
	averageCharPerWidth = int(width / fm.averageCharWidth());
}

FontAdapter::FontAdapter(int mh, int mah, double mfp)
	: minheight(mh), maxheight(mah), minimumFontPercent(mfp)
{
#ifdef  Q_OS_WIN
	minimumFontPercent = mfp * 0.6;
#endif //  Q_OS_WIN
}
void FontAdapter::reset(int mh, int Mh, double mfp)
{
	minheight = mh;
	maxheight = Mh;
	minimumFontPercent = mfp;
	*_generalFont = QFont(makeFont(1.0));
}
int FontAdapter::howMuchCharacterFitsIntoScreen()
{
	return averageCharPerWidth;
}
FontAdapter* FontAdapter::_instanse = Q_NULLPTR;
QFont* FontAdapter::_generalFont = Q_NULLPTR;
FontAdapter* FontAdapter::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new FontAdapter(AppSettings->fontMinHeight, AppSettings->fontMaxHeight,
			AppSettings->fontPercent);
	}
	return _instanse;
}

const QFont* FontAdapter::general()
{
	if (_generalFont == Q_NULLPTR)
	{
		_generalFont = new QFont(FontAdapter::makeFont(1.0));
		FontAdapter::instanse()->_setCharPerWidth();
	}
	return _generalFont;
}

QFont FontAdapter::makeFont(double extrapercents)
{
    double currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
    currentHeight *= FontAdapter::instanse()->minimumFontPercent;
	currentHeight *= extrapercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
    return QFont("Times new Roman", int(currentHeight));
}
QFont FontAdapter::makeCustomFont(double onlyPercents)
{
	double currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
	currentHeight *= onlyPercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
	return QFont("Times new Roman", int(currentHeight));
}
QString FontAdapter::breakStringToFitScreen(QString src, const QFontMetrics & metrics, double percent, int* breaks)
{
	double totalWidthAllowed = double(calculateAdaptiveWidth(percent)) - metrics.averageCharWidth();
	double tobreak = double(src.count()) * metrics.averageCharWidth() / totalWidthAllowed;
	if (tobreak <= 1.0)
	{
		if (breaks != Q_NULLPTR)
			*breaks = int(std::ceil(tobreak));
		return src;
	}
	QString out;
	out.reserve(src.count() + tobreak + 1);
	int wsum = 0;
	QString::iterator pos = src.begin();
	while (pos != src.end())
	{
		while (wsum < totalWidthAllowed && pos != src.end())
		{
			out += *pos;
			wsum += metrics.width(*pos);
			++pos;
			
		}
		wsum = 0;
		out += "\n";
	}
	if (breaks != Q_NULLPTR)
	{
		*breaks = int(std::ceil(tobreak));
	}
	return out;
}
QStringList _initDefaultStyles()
{
	QStringList styleSheets;
	styleSheets <<
		QStringLiteral("QPushButton {"     // StyleManager::stylesheets::OK_BUTTON
			"background-color: #a2deae;"
			"border: 1px solid gray;"
			"}")
			<<
		QStringLiteral("QPushButton {"		// BACK_BUTTON
			"background-color: #8ebde6;"
			"border: 1px solid gray;"
			"}")
		<<
		QStringLiteral("QPushButton{ "		//	COMMIT_BUTTON
			"background-color: #a0e899;"
			"border: 1px solid gray;"
			"} "
			"QPushButton:disabled {"
			"background-color: #c0c0c0;"
			"border: 1px solid gray;"
			"}")
		<<
		QStringLiteral("QPushButton {"		// DELETE_BUTTON
			"background-color: #d16860;"
			"border: 1px solid gray;"
			"}")
		<<
		QStringLiteral("QPushButton {"     //CHANGE_BUTTON
			"background-color: #dfe687;"
			"border: 1px solid gray"
			";}"
			" QPushButton:disabled {"
			"background-color:#999373;}"
		)
		<<
		QStringLiteral("QPushButton:checked {"   // CHECKED_BUTTON
			"background-color: #797d42;"
			"border: 2px solid black;"
			"}")
		<<
		QStringLiteral("QPushButton {"		// UP_SPINBOX_BUTTON
			"border: 1px solid #e3dac3;"
			"border-top-left-radius: 50px;"
			"border-bottom-left-radius: 50px;"
			"background-color: #8ef55f;"
			"}")
		<<
		QStringLiteral("QPushButton {"		// DOWN_SPINBOX_BUTTON
			"border: 1px solid #e3dac3;"
			"border-top-right-radius: 50px;"
			"border-bottom-right-radius: 50px;"
			"background-color: #f05d5d;"
			"}")
		<<
											//CANCEL_BUTTON,
		QStringLiteral("QPushButton{"
			"background-color: #f5a37f; "
			"border: 1px solid gray;}")
		<<
											//	SETTINGS_BUTTON
		QStringLiteral("QPushButton {"
			"background-color: #9893c9; "
			"border: 1px solid gray;}")
		<<

		QStringLiteral("QPushButton{ "			//NAVIGATE_BUTTON
			"background-color: #c6abf5;"
			"border: 1px solid gray;"
			"}"
			"QPushButton:disabled {"
			"background-color:#7e7785;"
			"}")
		<<									


		QStringLiteral("QCalendarWidget QToolButton{"  //  BETTER_CALENDAR
			"height: 60px;"
			"width: 150px;"
			"color: white;"
			"font-size: 24px;"
			"icon-size: 56px, 56px;"
			"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
			"}"
			"QCalendarWidget QMenu{"
			"width: 150px;"
			"left: 20px;"
			"color: white;"
			"font-size: 18px;"
			"background-color: rgb(100, 100, 100);"
			"}"
			"QCalendarWidget QSpinBox{"
			"width: 150px;"
			"font-size:24px;"
			"color: white;"
			"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
			"selection-background-color: rgb(136, 136, 136);"
"selection-color: rgb(255, 255, 255);"
"}"
"QCalendarWidget QSpinBox::up-button{"
"subcontrol-origin: border;"
"subcontrol-position: top right;"
"width:65px;"
"}"
"QCalendarWidget QSpinBox::down-button{"
"subcontrol-origin: border;		"
"subcontrol-position: bottom right;"
"width:65px;"
"}"
"QCalendarWidget QSpinBox::up-arrow{"
"width:56px;"
"height:56px;"
"}"
"QCalendarWidget QSpinBox::down-arrow{"
"width:56px;"
"height:56px;"
"}"
"QCalendarWidget QWidget{"
"alternate-background-color: rgb(128, 128, 128);"
"}"
"QCalendarWidget QAbstractItemView : enabled"
"{"
"font-size:24px;"
"color: rgb(180, 180, 180);"
"background-color: black;"
"selection-background-color: rgb(64, 64, 64);"
"selection-color: rgb(0, 255, 0);"
"}"
"QCalendarWidget QWidget#qt_calendar_navigationbar"
"{"
"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
"}"
"QCalendarWidget QAbstractItemView : disabled"
"{"
"color: rgb(64, 64, 64);"
"}")
<<
QStringLiteral("QLabel {color: red;}")	// ERROR_LABEL
<<
QStringLiteral("QPushButton{"			// UNCHECKED_BUTTON
	"background-color: #f5a37f; "
	"border: 1px solid gray;}")
	<<

	QStringLiteral("QSpinBox {border: 2px solid blue;}")		// LISTENING_CONTROL
	<<
	QStringLiteral("QPushButton{ "  //  CHECKBOX_BUTTON
		"background-color: #f5a4bc;"
		"border: 1px solid gray;"
		"}"
		"QPushButton:checked {"
		"background-color:#d2e092;"
		"border: 1px solid black;"
		"}")
	<<
	QStringLiteral(						//	FOCUSED_SPINBOX
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; }"
	)
	<<
	QStringLiteral(							//	FOCUSED_DATETIMEEDIT
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; } "
		"QDateEdit::drop-down {"
		"subcontrol-origin: padding;"
		"subcontrol-position: center right;"
		"width: %1 px;"
		"}"
	)
	<<
	QStringLiteral(						//FOCUSED_LINEEDIT
		"QLineEdit:focus {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; } "
	);
	return styleSheets;
}


StyleManager::StyleManager()
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("stylesheets");
	QString buffer;
	buffer.reserve(int(stylesheets::STYLESHEET_LIST_END));
	bool ok = true;
	ok &= !(stockedStyles << settings.value("OK_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("BACK_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("COMMIT_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("DELETE_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("CHANGE_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("CHECKED_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("UP_SPINBOX_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("DOWN_SPINBOX_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("CANCEL_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("SETTINGS_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("NAVIGATE_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("BETTER_CALENDAR").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("ERROR_LABEL").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("UNCHECKED_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("CHECKBOX_BUTTON").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("LISTENING_CONTROL").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("FOCUSED_SPINBOX").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("FOCUSED_DATETIMEEDIT").toString()).last().isEmpty();
	ok &= !(stockedStyles << settings.value("FOCUSED_LINEEDIT").toString()).last().isEmpty();
	if (!ok)
	{
		stockedStyles = _initDefaultStyles();
	}
	EnumerablesFieldsColor = settings.value("EnumerableFieldsColor", QColor(230, 255, 230)).value<QColor>();
	settings.endGroup();
}
StyleManager* StyleManager::_instanse = Q_NULLPTR;
const StyleManager* StyleManager::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new StyleManager();
	}
	return _instanse;
}

const QString& StyleManager::getStyle(stylesheets s)
{
	return instanse()->stockedStyles[int(s)];
}

void StyleManager::pushCurrentStylesheets()
{
	if (instanse()->stockedStyles.isEmpty() || instanse()->stockedStyles.count() != int(stylesheets::STYLESHEET_LIST_END))
	{
		_instanse->stockedStyles = _initDefaultStyles();
	}
	QSettings settings("settings.ini", QSettings::IniFormat);
	settings.beginGroup("stylesheets"); settings.setValue("OK_BUTTON", getStyle(stylesheets::OK_BUTTON));
	settings.setValue("BACK_BUTTON", getStyle(stylesheets::BACK_BUTTON));
	settings.setValue("COMMIT_BUTTON", getStyle(stylesheets::COMMIT_BUTTON));
	settings.setValue("DELETE_BUTTON", getStyle(stylesheets::DELETE_BUTTON));
	settings.setValue("CHANGE_BUTTON", getStyle(stylesheets::CHANGE_BUTTON));
	settings.setValue("CHECKED_BUTTON", getStyle(stylesheets::CHECKED_BUTTON));
	settings.setValue("UP_SPINBOX_BUTTON", getStyle(stylesheets::UP_SPINBOX_BUTTON));
	settings.setValue("DOWN_SPINBOX_BUTTON", getStyle(stylesheets::DOWN_SPINBOX_BUTTON));
	settings.setValue("CANCEL_BUTTON", getStyle(stylesheets::CANCEL_BUTTON));
	settings.setValue("SETTINGS_BUTTON", getStyle(stylesheets::SETTINGS_BUTTON));
	settings.setValue("NAVIGATE_BUTTON", getStyle(stylesheets::NAVIGATE_BUTTON));
	settings.setValue("BETTER_CALENDAR", getStyle(stylesheets::BETTER_CALENDAR));
	settings.setValue("ERROR_LABEL", getStyle(stylesheets::ERROR_LABEL));
	settings.setValue("UNCHECKED_BUTTON", getStyle(stylesheets::UNCHECKED_BUTTON));
	settings.setValue("CHECKBOX_BUTTON", getStyle(stylesheets::CHECKBOX_BUTTON));
	settings.setValue("LISTENING_CONTROL", getStyle(stylesheets::LISTENING_CONTROL));
	settings.setValue("FOCUSED_SPINBOX", getStyle(stylesheets::FOCUSED_SPINBOX));
	settings.setValue("FOCUSED_DATETIMEEDIT", getStyle(stylesheets::FOCUSED_DATETIMEEDIT));
	settings.setValue("FOCUSED_LINEEDIT", getStyle(stylesheets::FOCUSED_LINEEDIT));
	settings.endGroup();
}
