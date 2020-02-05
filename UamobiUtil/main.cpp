#include "UamobiUtil.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
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



int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	AppSettings->timeoutInt = 10000;					//	while not actual version - timeout is maximized to avoid testing delays
	AppSettings->setTranslator();
	AppSettings->HttpUrl = "http://92.115.237.226:60080/um/unitest.php";
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
	UamobiUtil w;
	w.show();
#endif
	return a.exec();
}