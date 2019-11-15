#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "networking/StaticTestingDataEngine.h"
#ifdef Q_OS_WINCE
#include <QtCore/QTimer>
#endif
#include "networking/Parsers/DynamicLinearParsers.h"
#include "debugtrace.h"
//#define TESTING

/*
		This is uamobiUtil app, which is suited to replace old one. Main idea is the same.
		This realisation is Qt5-based, with guarding macros to allow legacy-compatibility with qt4.
		All new elements must be ready to compile under qt4\CE, so try to provide legacy versions.

		Legenda:
			P'%s' ->encloses any data structure that is parsed from net response. They are stored at RequestParser file;
			__ASSOCIATED_DATABASE_FUNCTION__  -> this means that this function is used by provided code
			__OPTIONAL_REPLACEMENT_DBFUNCTION__ -> this means that this fuction can replace upper database function in some cases

*/
//#define TESTING

QString p = "<?xml version=\"1.0\" encoding=\"CP1251\"?>"
"<serverresponse>"
"<announcement>"
"<status>200</status>"
"<type>list</type>"
"<entrysize>2</entrysize>"
"<entryname>stillage</entryname>"
"</announcement>"
"<entries>"
"<e>"
"<e1>205333065</e1>"
"<e2>Vata CL 4-8</e2>"
"</e>"
"</entries>"
"</serverresponse>";


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	GlobalAppSettings global;	//	global settings object
#ifdef TESTING
	global.networkingEngine = new StaticTestingDataEngine(100);	//	It is way better to use for testing a static engine with custom delay
#else		//	actual db
	global.networkingEngine = new HttpUpdateEngine(global.HttpUrl, &a);
#endif
	global.timeoutInt = 10000;					//	while not actual version - timeout is maximized to avoid testing delays
	global.setTranslator();
	detrace_METHEXPL("making parsing: " << makeUPResSnapshot(SerializedListParser(p, p).read()));
#ifdef Q_OS_WINCE
	setCurrentSize(calculateAdaptiveSize(1, 0.9));
	UamobiUtil w(global);
	w.move(0, 0);
	w.showMaximized();
	w.raise();
	w.setFixedHeight(calculateAdaptiveButtonHeight(0.9));
	w.setFixedWidth(calculateAdaptiveWidth(1));
	QTimer::singleShot(0, &w, SLOT(showMaximized()));
#else
	setCurrentSize(calculateAdaptiveSize(1));
	UamobiUtil w(global);
	w.show();
#endif
	return a.exec();
}