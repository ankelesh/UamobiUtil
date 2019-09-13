#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "networking/StaticTestingDataEngine.h"

#define TESTING

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GlobalAppSettings global;
#ifdef TESTING
	global.networkingEngine = new StaticTestingDataEngine(100);
#else
	global.HttpUrl = "una.md:3323/um/romina.php";
	global.networkingEngine = new HttpUpdateEngine(global.HttpUrl, &a);
#endif
	global.timeoutInt = 2000000000000;
	a.installTranslator(&global.translator);
	UamobiUtil w(global);
	w.showMaximized();
	return a.exec();
}
