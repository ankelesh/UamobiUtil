#include "ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif
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
const QString OK_BUTTONS_STYLESHEET(QStringLiteral("QPushButton {"
	"background-color: #a2deae;"
	"border: 1px solid gray;"
	"}"));

const QString BACK_BUTTONS_STYLESHEET(QStringLiteral("QPushButton {"
	"background-color: #8ebde6;"
	"border: 1px solid gray;"
	"}"));

const QString COMMIT_BUTTONS_STYLESHEET(QStringLiteral("QPushButton{ "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"} "
	"QPushButton:disabled {"
	"background-color:#4a7039;"
	"}"));

const QString DELETE_BUTTONS_STYLESHEET(QStringLiteral("QPushButton {"
	"background-color: #d16860;"
	"border: 1px solid gray;"
	"}"));

const QString CHANGE_BUTTONS_STYLESHEET(QStringLiteral("QPushButton {"
	"background-color: #dfe687;"
	"border: 1px solid gray"
	";}"
	" QPushButton:disabled {"
	"background-color:#999373;}"
));

const QString CHECKED_BUTTONS_STYLESHEET(QStringLiteral("QPushButton:checked {"
	"background-color: #797d42;"
	"border: 2px solid black;"
	"}"));

const QString UP_SPINBOX_STYLESHEET(QStringLiteral("QPushButton {"
	"border: 1px solid #e3dac3;"
	"border-top-left-radius: 50px;"
	"border-bottom-left-radius: 50px;"
	"background-color: #8ef55f;"
	"}"));

const QString DOWN_SPINBOX_STYLESHEET(QStringLiteral("QPushButton {"
	"border: 1px solid #e3dac3;"
	"border-top-right-radius: 50px;"
	"border-bottom-right-radius: 50px;"
	"background-color: #f05d5d;"
	"}"));


const QString LARGE_BUTTON_STYLESHEET(QStringLiteral("QDateEdit{"
	"border-radius: 5px;"
	"padding: 1px 10px 1px 5px;"
	"border: 1px solid black;"
	"min-height: 30px;"
	"}"
	"QDateEdit::drop-down{"
	"subcontrol-origin: padding;"
	"subcontrol-position: top right;"
	"border-left-width: 1px;"
	"width: 55px;"
	"border-left-style: solid;"
	"border-top-right-radius: 3px;"
	"border-bottom-right-radius: 3px;"
	"border-left-color: black;"
	"image: url(:/res/downarrow.png);"
	"}"
	"QDateEdit::down-arrow{"
	"height:30px;"
	"width:30px;"
	"}"));

const QString BETTER_CALENDAR_STYLESHEET = QStringLiteral("QCalendarWidget QToolButton{"
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
	"}");
const QString ERROR_TEXT_STYLESHEET = QStringLiteral("QLabel {color: red;}");

const QString CANCEL_BUTTONS_STYLESHEET = QStringLiteral("QPushButton{"
	"background-color: #f5a37f; "
	"border: 1px solid gray;}");
;

const QString SETTINGS_BUTTONS_STYLESHEET = QStringLiteral("QPushButton {"
	"background-color: #9893c9; "
	"border: 1px solid gray;}");

const QString UNCHECKED_BUTTONS_STYLESHEET = QStringLiteral("QPushButton{"
	"background-color: #e9edb4; "
	"border: 1px solid gray;}");

const QString NAVIGATE_BUTTONS_STYLESHEET = QStringLiteral("QPushButton{ "
	"background-color: #c6abf5;"
	"border: 1px solid gray;"
	"}"
	"QPushButton:disabled {"
	"background-color:#7e7785;"
	"}");
const QString LISTENING_CONTROL_STYLESHEET = QStringLiteral("QSpinBox {border: 2px solid blue;}");

const QString ZEBRAEVEN_BUTTONS_STYLESHEET(
	QStringLiteral("QPushButton {background-color: #f8fced; border: 1px solid gray;}"));

const QString ZEBRAODD_BUTTONS_STYLESHEET(QStringLiteral("QPushButton {background-color: #878a7f; border: 1px solid gray;}"));

const QString CHECKBOX_BUTTON_STYLESHEET(
	QStringLiteral("QPushButton{ "
		"background-color: #f5a4bc;"
		"border: 1px solid gray;"
		"}"
		"QPushButton:checked {"
		"background-color:#d2e092;"
		"border: 1px solid black;"
		"}")
);

const QString FOCUSED_SPINBOX_STYLESHEET(
	QStringLiteral(
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; }"
	)
);

const QString FOCUSED_DATETIMEEDIT_STYLESHEET(
	QStringLiteral(
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
);
const QString FOCUSED_LINEEDIT_STYLESHEET(
	QStringLiteral(
		"QLineEdit:focus {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; } "
	)
);

const QFont & makeFont(double /*perc*/)
{
	return *(FontAdapter::general());
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
	}
	return _generalFont;
}

QFont FontAdapter::makeFont(double extrapercents)
{
	int currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
	currentHeight *= FontAdapter::instanse()->minimumFontPercent;
	currentHeight *= extrapercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
	return QFont("Times new Roman", currentHeight);
}
