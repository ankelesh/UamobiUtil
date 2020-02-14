#include "abs_control.h"
#include "QuantityControl.h"
#include "ControlTranslator.h"
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif



abs_control::abs_control(QWidget* parent, controlType Type, QString cname)
	: QObject(parent), type(Type), parent(parent),name(cname), label(makeControlTranslation(cname))
{
}

QString abs_control::getValue() const
{
	return prepareAndReturnValue();
}

QWidget* abs_control::myWidget() const
{
	return getInnerWidget();
}

abs_control::controlType abs_control::myType()
{
	return type;
}

bool abs_control::setValue(QString val)
{
	return parseAndSetValue(val);
}

void abs_control::reset()
{
	clear();
}

abs_control::~abs_control()
{
}

void abs_control::subcontrolEditingFinished()
{
	if (valueAvailable())
	{
		emit editingFinished();
		emit valueReceived(prepareAndReturnValue());
	}
}


QString chopZeros(QString val, bool cdot)
// removes .0000 from end of the line to avoid cursor-to-end behaviour
{
	val.replace(',', '.');
	if (cdot)
	{
		while (val.endsWith('0'))
		{
			val.chop(1);
		}
	}
	else
	{
		while (val.endsWith('0') || val.endsWith('.'))
		{
			val.chop(1);
		}
	}
	return val;
}
