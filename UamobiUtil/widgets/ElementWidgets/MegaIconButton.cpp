#include "MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"

#ifdef QT_VERSION5X
#include <QtWidgets/qstyle.h>
#else
#include <QtGui/QStyle>
#endif
#include <qstyleoption.h>
#include <qstylepainter.h>

void MegaIconButton::paintEvent(QPaintEvent*)
// This paintEvent is modified with aligns to make icon dominating. ImageCoefficient is defining which
// percent of the button will be occupied with icon.
{
	QStylePainter spainter(this);
	QStyleOptionButton opt;
	this->initStyleOption(&opt);
	spainter.drawPrimitive(QStyle::PE_Widget, opt);
	opt.text = "";
	spainter.drawControl(QStyle::CE_PushButton, opt);
    spainter.drawItemPixmap(this->rect(), Qt::AlignCenter, this->ico.pixmap(int(this->rect().height() * imageCoefficient)));
	spainter.drawItemText(this->rect(), Qt::AlignBottom | Qt::AlignCenter, QPalette(), true, text(), QPalette::NoRole);
}
//	Changed default imgCoef 0.7 > 0.6 for better view
MegaIconButton::MegaIconButton(QWidget* parent)
	: QPushButton(parent), imageCoefficient(0.6)
{
	setMinimumHeight(calculateAdaptiveButtonHeight());	//	Now adaptive height is used by default
#ifdef Q_OS_WINCE
	setFont(AppFonts->makeFont(0.7));
#endif
}

MegaIconButton::MegaIconButton(QIcon ic, const double imageCoef, const QString txt, QWidget* parent)
	: QPushButton(txt, parent), ico(ic), imageCoefficient(imageCoef)
{
	setMinimumHeight(calculateAdaptiveButtonHeight());
#ifdef Q_OS_WINCE
	setFont(AppFonts->makeFont(0.7));
#endif
}

void MegaIconButton::setIcon(QIcon icon)
{
	ico = icon;
}

void MegaIconButton::setScaleCoefficient(double coe)
{
	imageCoefficient = coe;
}
