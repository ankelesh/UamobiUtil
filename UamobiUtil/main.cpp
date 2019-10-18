#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "networking/StaticTestingDataEngine.h"

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

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	GlobalAppSettings global;	//	global settings object
#ifdef TESTING
	global.networkingEngine = new StaticTestingDataEngine(100);	//	It is way better to use for testing a static engine with custom delay
#else
    global.HttpUrl = "http://una.md:3323/um/bonus2.php";			//	actual db
	global.networkingEngine = new HttpUpdateEngine(global.HttpUrl, &a);
#endif
    global.timeoutInt = 3000;					//	while not actual version - timeout is maximized to avoid testing delays
	global.setTranslator();
	UamobiUtil w(global);
	w.showMaximized();
	return a.exec();
}
