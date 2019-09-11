#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "networking/StaticTestingDataEngine.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GlobalAppSettings global;
	global.networkingEngine = new StaticTestingDataEngine(100);
	global.timeoutInt = 2000000000000;
	a.installTranslator(&global.translator);
	UamobiUtil w(global);
	w.showMaximized();
	return a.exec();
}
