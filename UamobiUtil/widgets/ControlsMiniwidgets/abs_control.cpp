#include "abs_control.h"

abs_control::abs_control(controlType Type, QWidget* parent)
	:QWidget(parent), mainLayout(new QHBoxLayout(this)),
	type(Type), isAwaiting(false)
{
	this->setLayout(mainLayout);
}

QString abs_control::getValue() const
{
	return prepareAndReturnValue();
}

bool abs_control::canGiveValue() const
{
	return valueAvailable();
}

controlType abs_control::myType()
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
	show();
}

void abs_control::reset()
{
	clear();
	isAwaiting = false;
}
