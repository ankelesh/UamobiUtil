#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GlobalAppSettings global;
	global.networkingEngine = new HttpUpdateEngine(global.HttpUrl, &a);
	HttpUpdateEngine* temp = static_cast<HttpUpdateEngine*>(global.networkingEngine);
	temp->setUrl("una.md:3323/um/romina.php");
	global.timeoutInt = 2000000000000;
	a.installTranslator(&global.translator);
	UamobiUtil w(global);
	w.showMaximized();
	return a.exec();
}
