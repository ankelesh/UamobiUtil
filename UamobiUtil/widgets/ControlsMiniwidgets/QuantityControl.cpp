#include "QuantityControl.h"

#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
QString QuantityControl::prepareAndReturnValue() const
{
	return QString::number(innerSpinbox->value());
}

bool QuantityControl::parseAndSetValue(QString str)
{
	bool ok;
	int toSet = str.toInt(&ok);
	if (ok)
	{
		innerSpinbox->setValue(toSet);
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
	return innerSpinbox->value() > 0;
}

bool QuantityControl::hasFocus() const
{
	return innerSpinbox->hasFocus();
}

QuantityControl::QuantityControl(QString& cname,QWidget* parent)
	: 
	abs_control(cname, Int),innerSpinbox(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(99999);
	innerSpinbox->setValue(0);
	hide();
}

QuantityControl::QuantityControl( QString& assocBuffer, QString& cname, QWidget* parent)
	:abs_control(cname, assocBuffer, Int), innerSpinbox(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(99999);
	innerSpinbox->setValue(0);
	hide();
}

QuantityControl::~QuantityControl()
{
	innerSpinbox->hide();
	innerSpinbox->blockSignals(true);
	innerSpinbox->deleteLater();
}

void QuantityControl::setFocus() const
{
	innerSpinbox->setFocus();
}

void QuantityControl::show()
{
	innerSpinbox->show();
}

void QuantityControl::hide()
{
	innerSpinbox->hide();
}

void QuantityControl::refresh()
{
#ifdef DEBUG
	detrace_METHCALL("QuantityControl::refresh |" << associatedBuffer << "| of QC named " << name);
#endif
	bool ok;
	int toSet = associatedBuffer->toInt(&ok);
	if (ok)
	{
		innerSpinbox->setValue(toSet);
	}
}

void QuantityControl::installEventFilter(QObject* keyfilter)
{
	innerSpinbox->installEventFilter(keyfilter);
}
