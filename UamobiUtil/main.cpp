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
"<searchresult>"
"<status>200</status>"
"<page>"
"<from>1</from>"
"<to>1</to>"
"<last>true</last>"
"</page>"
"<result>"
"<barcode>2000000442426</barcode>"
"<cmid>52524213</cmid>"
"<qty>1</qty>"
"<title>Mingea de fotbal"
"2000000442426"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>2000000450872</barcode>"
"<cmid>52524214</cmid>"
"<qty>1</qty>"
"<title>Forme p/u biscuiti, metal 3buc"
"2000000450872"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>2000000431949</barcode>"
"<cmid>52524215</cmid>"
"<qty>1</qty>"
"<title>Fringhie cauciuc p/u uscat 20м"
"2000000431949"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>2000000431857</barcode>"
"<cmid>52524216</cmid>"
"<qty>1</qty>"
"<title>Corector RETYPE"
"2000000431857"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>2000000431857</barcode>"
"<cmid>52524217</cmid>"
"<qty>1</qty>"
"<title>Corector RETYPE"
"2000000431857"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>2000000442365</barcode>"
"<cmid>52524219</cmid>"
"<qty>1</qty>"
"<title>Pompa p/u bazin"
"2000000442365"
"Накоплено: 1  </title>"
"</result>"
"<result>"
"<barcode>4840167001399</barcode>"
"<cmid>52524220</cmid>"
"<qty>1</qty>"
"<title>INCOMLAC Unt  CIOCOLATA 62% 200g"
"4840167001399"
"Накоплено: 1  </title>"
"</result>"
"<allowdelete>true</allowdelete></searchresult>";


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