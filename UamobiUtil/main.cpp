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
#include <QProxyStyle>
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
class WinCEProxyStyle : public QProxyStyle
{
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        if (metric == QStyle::PM_ButtonMargin)
            return baseStyle()->pixelMetric(metric, option, widget)*2.5;
        /*else if (metric == QStyle::PM_LayoutLeftMargin
                          || metric == QStyle::PM_LayoutTopMargin
                          || metric == QStyle::PM_LayoutRightMargin
                          || metric == QStyle::PM_LayoutBottomMargin
                          || metric == QStyle::PM_LayoutHorizontalSpacing
                          || metric == QStyle::PM_LayoutVerticalSpacing)
                return 2;*/
        else
            return baseStyle()->pixelMetric(metric, option, widget);
    }
};


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	AppSettings->timeoutInt = 10000;					//	while not actual version - timeout is maximized to avoid testing delays
	AppSettings->setTranslator();
	AppSettings->HttpUrl = "una.md:3323/um/sammy0520181.php";
#ifdef Q_OS_WINCE
    UamobiUtil w;
	w.move(0, 0);
    a.setStyle(new WinCEProxyStyle());
    w.showNormal();
    w.raise();
    w.move(0, -24);
    w.setWindowFlags((w.windowFlags() | Qt::FramelessWindowHint | Qt::CustomizeWindowHint)
                                  & ~(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint
                                       | Qt::WindowCloseButtonHint | Qt::Dialog | Qt::WindowContextHelpButtonHint));
    w.setFixedHeight(calculateAdaptiveButtonHeight(1));
	w.setFixedWidth(calculateAdaptiveWidth(1));
    QTimer::singleShot(1, &w, SLOT(showMaximized()));
#else
	UamobiUtil w;
	w.show();
#endif
	return a.exec();
}
