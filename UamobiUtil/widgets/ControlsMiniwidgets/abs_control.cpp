#include "abs_control.h"
#include "QuantityControl.h"
#include "ControlTranslator.h"
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif



abs_control::abs_control(QString  cname, QString& assocBuffer, controlType Type)
	:associatedBuffer(&assocBuffer),
	type(Type), isAwaiting(false), defaultBuffer(), name(cname)
{
	label = makeControlTranslation(cname);
}
abs_control::abs_control(QString cname, controlType Type)
	: associatedBuffer(&defaultBuffer),
	type(Type), isAwaiting(false), defaultBuffer(), name(cname), label()
{
	label = makeControlTranslation(cname);
}
QString abs_control::getValue() const
{
	return prepareAndReturnValue();
}

bool abs_control::canGiveValue() const
{
	return valueAvailable();
}

abs_control::controlType abs_control::myType()
{
	return type;
}

bool abs_control::setValue(QString val)
{
	return parseAndSetValue(val);
}

bool abs_control::isAwaitingValue()
{
	return isAwaiting;
}

void abs_control::setAwaiting()
{
	isAwaiting = true;
}

void abs_control::reset()
{
	clear();
	isAwaiting = false;
}

void abs_control::show()
{
}

void abs_control::hide()
{
}

void abs_control::unassociateBuffer()
{
	associatedBuffer = &defaultBuffer;
    setListening(false);
	isAwaiting = false;
}

void abs_control::associateBuffer(QString& buffer)
{
	associatedBuffer = &buffer;
    setListening(true);
	isAwaiting = true;
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
void control_utils::refresher::sync(QString& sval)
{
#ifdef DEBUG
	detrace_METHCALL("refresher::sync(" << sval << ")");
#endif
	if (bufferPointer != Q_NULLPTR)
	{
		if (spbox != Q_NULLPTR)
		{
			if (spbox->hasFocus())
			{
				(*(*bufferPointer)) = chopZeros(sval, (*bufferPointer)->contains('.'));
#ifdef DEBUG
				detrace_METHEXPL("after sync: " << (*(*bufferPointer)));
#endif
			}
		}
	}
}
