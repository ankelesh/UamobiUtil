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
#include "widgets/utils/ElementsStyles.h"
/*
		This is uamobiUtil app, which is suited to replace old one. Main idea is the same.
		This realisation is Qt5-based, with guarding macros to allow legacy-compatibility with qt4.
		All new elements must be ready to compile under qt4\CE, so try to provide legacy versions.

*/
//#define TESTING
class WinCEProxyStyle : public QProxyStyle
{
    
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        if (metric == QStyle::PM_ButtonMargin)
            return int(baseStyle()->pixelMetric(metric, option, widget)*2.5);
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
#ifdef Q_OS_ANDROID
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	QApplication a(argc, argv);
	debugtrace::init(detr_supply::all, detr_supply::file, QVector<OutputMode>(), 120, false);
    detrace_APPSTART(VERSION, SUFFIX);
	AppSettings->setTranslator();
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
#ifdef Q_OS_ANDROID
    a.setDoubleClickInterval(900);
    w.setFixedSize(calculateAdaptiveSize(1.0));
#endif

	return a.exec();
}
