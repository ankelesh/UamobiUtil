#include "QuantityControl.h"
#include "widgets/utils/ElementsStyles.h"
//#define DEBUG
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#if defined(QT_VERSION5X) && defined(Q_OS_ANDROID)
#include <QApplication>
#include <qinputmethod.h>
#endif
QString QuantityControl::prepareAndReturnValue() const
{
    return QString::number(innerSpinbox->dvalue());
}

bool QuantityControl::parseAndSetValue(QString str)
{
	bool ok;
	double toSet = str.toDouble(&ok);
	if (ok)
	{
		innerSpinbox->setDValue(toSet);
		return true;
	}
	return false;
}

void QuantityControl::clear()
{
	innerSpinbox->setValue(0);
}

bool QuantityControl::valueAvailable() const
{
	return innerSpinbox->dvalue() > 0;
}


QuantityControl::QuantityControl(bool isInt, const QString& cname, QWidget* Parent)
    :
    abs_control(Parent, ((isInt)?    InputControlEntity::Int : InputControlEntity::Float), cname),
	innerSpinbox(new BigButtonsSpinbox(
	((isInt)? BigButtonsSpinbox::intspin : BigButtonsSpinbox::floatspin), parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(10000);
	innerSpinbox->setValue(0);
	innerSpinbox->setInfo(label);
	innerSpinbox->setStyleSheet(FOCUSED_SPINBOX_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(innerSpinbox, &BigButtonsSpinbox::returnPressed, this, &QuantityControl::subcontrolEditingFinished);
#else
	QObject::connect(innerSpinbox, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
	hide();
}



QuantityControl::~QuantityControl()
{
	innerSpinbox->hide();
	innerSpinbox->blockSignals(true);
	innerSpinbox->deleteLater();
}

QWidget* QuantityControl::getInnerWidget() const
{
	return innerSpinbox;
}

void QuantityControl::setFocus() const
{
	innerSpinbox->setFocus();
	innerSpinbox->selectAll();
#if defined(QT_VERSION5X) && defined(Q_OS_ANDROID)
	qApp->inputMethod()->show();
#endif
}

void QuantityControl::show()
{
	innerSpinbox->show();
}

void QuantityControl::hide()
{
	innerSpinbox->hide();
}

