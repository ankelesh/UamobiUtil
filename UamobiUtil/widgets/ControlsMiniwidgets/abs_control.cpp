#include "abs_control.h"
#include "QuantityControl.h"
#include "ControlTranslator.h"



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


