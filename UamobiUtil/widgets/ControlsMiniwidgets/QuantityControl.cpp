#include "QuantityControl.h"
#include "widgets/utils/ElementsStyles.h"
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
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


QuantityControl::QuantityControl(bool isInt, QString& cname, QWidget* parent)
	:
	abs_control(parent, ((isInt)? Int : Float), cname),
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
}

void QuantityControl::show()
{
	innerSpinbox->show();
}

void QuantityControl::hide()
{
	innerSpinbox->hide();
}

