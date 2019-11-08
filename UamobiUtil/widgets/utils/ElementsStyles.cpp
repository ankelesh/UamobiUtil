#include "ElementsStyles.h"
#ifdef DEBUG
#include "debugtrace.h"
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
const QString OK_BUTTONS_STYLESHEET("QPushButton { \
			background-color: #a2deae;		\
			border: 1px solid gray;			\
		 }");

const QString BACK_BUTTONS_STYLESHEET("QPushButton { \
			background-color: #8ebde6;		\
			border: 1px solid gray;			\
		 }");

const QString COMMIT_BUTTONS_STYLESHEET("QPushButton{ "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"} "
	"QPushButton:disabled {"
	"background-color:#4a7039;"
	"}");

const QString DELETE_BUTTONS_STYLESHEET("QPushButton { \
			background-color: #d16860;		\
			border: 1px solid gray;			\
		}");

const QString CHANGE_BUTTONS_STYLESHEET("QPushButton {\
			background-color: #dfe687;		\
			border: 1px solid gray			\
		;}");

const QString CHECKED_BUTTONS_STYLESHEET("QPushButton:checked {\
			background-color: #797d42;		\
			border: 2px solid black;		\
		}");

const QString UP_SPINBOX_STYLESHEET("QPushButton {	\
			border: 1px solid #e3dac3;		\
			border-top-left-radius: 50px;	\
			border-bottom-left-radius: 50px;\
			background-color: #8ef55f;		\
		}");

const QString DOWN_SPINBOX_STYLESHEET("QPushButton {\
			border: 1px solid #e3dac3;		\
			border-top-right-radius: 50px;	\
			border-bottom-right-radius: 50px;\
			background-color: #f05d5d;		\
		}");

const QString LARGE_BUTTON_STYLESHEET("QDateEdit{	\
			border-radius: 5px;				\
			padding: 1px 10px 1px 5px;		\
			border: 1px solid black;		\
			min-height: 30px;				\
		}\
		QDateEdit::drop-down{				\
			subcontrol-origin: padding;		\
			subcontrol-position: top right;	\
			border-left-width: 1px;			\
			width: 55px;					\
			border-left-style: solid;		\
			border-top-right-radius: 3px;	\
			border-bottom-right-radius: 3px;\
			border-left-color: black;		\
			image: url(:/res/downarrow.png);\
		}\
		QDateEdit::down-arrow{				\
			height:30px;					\
			width:30px;						\
		}");

const QString BETTER_CALENDAR_STYLESHEET = "QCalendarWidget QToolButton{\
			height: 60px;					\
			width: 150px;					\
			color: white;					\
			font-size: 24px;				\
			icon-size: 56px, 56px;			\
			background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);\
		}\
		QCalendarWidget QMenu{				\
			width: 150px;					\
			left: 20px;						\
			color: white;					\
			font-size: 18px;				\
			background-color: rgb(100, 100, 100);\
		}\
		QCalendarWidget QSpinBox{			\
			width: 150px;					\
			font-size:24px;					\
			color: white;					\
			background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);\
			selection-background-color: rgb(136, 136, 136);\
			selection-color: rgb(255, 255, 255);\
		}\
		QCalendarWidget QSpinBox::up-button{\
			subcontrol-origin: border;		\
			subcontrol-position: top right; \
			width:65px;						\
		}\
		QCalendarWidget QSpinBox::down-button{\
			subcontrol-origin: border;		\
			subcontrol-position: bottom right;\
			width:65px;						\
		}\
		QCalendarWidget QSpinBox::up-arrow{ \
			width:56px;						\
			height:56px;					\
		}\
		QCalendarWidget QSpinBox::down-arrow{\
			width:56px;						\
			height:56px;					\
		}\
		QCalendarWidget QWidget{			\
			alternate-background-color: rgb(128, 128, 128);\
		}\
		QCalendarWidget QAbstractItemView : enabled	\
		{\
			font-size:24px;					\
			color: rgb(180, 180, 180);		\
			background-color: black;		\
			selection-background-color: rgb(64, 64, 64);\
			selection-color: rgb(0, 255, 0);\
		}\
		QCalendarWidget QWidget#qt_calendar_navigationbar\
		{\
			background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);\
		}\
		QCalendarWidget QAbstractItemView : disabled\
		{\
			color: rgb(64, 64, 64);			\
		}";
const QString ERROR_TEXT_STYLESHEET = "QLabel {color: red;}";

const QString CANCEL_BUTTONS_STYLESHEET = "QPushButton{"
"background-color: #f5a37f; "
"border: 1px solid gray;}";
;

const QString SETTINGS_BUTTONS_STYLESHEET = "QPushButton {"
"background-color: #9893c9; "
"border: 1px solid gray;}";

const QString UNCHECKED_BUTTONS_STYLESHEET = "QPushButton{"
"background-color: #e9edb4; "
"border: 1px solid gray;}";

const QString NAVIGATE_BUTTONS_STYLESHEET = "QPushButton{ "
"background-color: #c6abf5;"
"border: 1px solid gray;"
"}"
"QPushButton:disabled {"
"background-color:#7e7785;"
"}";
const QString LISTENING_CONTROL_STYLESHEET="QSpinBox {border: 2px solid blue;}";
static QSize currsize;
const QSize& getCurrentSize()
{
#ifdef DEBUG
	detrace_METHCALL("getCurrentSize, " << currsize.height() << currsize.width());
#endif

	return currsize;
}

void setCurrentSize(const QSize& sz)
{
#ifdef DEBUG
	detrace_METHCALL("setCurrentSize, " << sz.height() << sz.width());
#endif
	currsize = sz;
}

QString countAdaptiveFont(double perc)
{
	return " font-size: " + QString::number((int)(GEOMETRY_SOURCE->availableGeometry().height() * perc)) + "px;";
}

QFont makeFont(double perc)
{
	return QFont("Arial", GEOMETRY_SOURCE->availableGeometry().height() * perc);
}

const QString ZEBRAEVEN_BUTTONS_STYLESHEET(
	"QPushButton {background-color: #f8fced; border: 1px solid gray;}");
const QString ZEBRAODD_BUTTONS_STYLESHEET("QPushButton {background-color: #878a7f; border: 1px solid gray;}");
const QString CHECKBOX_BUTTON_STYLESHEET(
	"QPushButton{ "
	"background-color: #f5a4bc;"
	"border: 1px solid gray;"
	"}"
	"QPushButton:checked {"
	"background-color:#d2e092;"
	"border: 1px solid black;"
	"}"
);
