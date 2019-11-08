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
}

void abs_control::associateBuffer(QString& buffer)
{
	associatedBuffer = &buffer;
    setListening(true);
}

abs_control* fabricateControl(QString& assocBuffer, QString initstr, QBoxLayout* layout, QWidget* parent)
{
	switch (initstr.length())
	{
	case 3:
		if (initstr == "qty") {
			QuantityControl* qc = new QuantityControl(assocBuffer, initstr, parent);
			layout->insertWidget(layout->count() - 1, qc->myWidget());
			return qc;
		}
	case 7:
		if (initstr == "addqty") {
			QuantityControl* qc = new QuantityControl(assocBuffer, initstr, parent);
			layout->insertWidget(layout->count() - 1, qc->myWidget());
			return qc;
		}
	}
	return Q_NULLPTR;
}

abs_control* fabricateControl(QString initstr, QBoxLayout* layout, QWidget* parent)
{
	switch (initstr.length())
	{
	case 3:
		if (initstr == "qty")
		{
			QuantityControl* qc = new QuantityControl(initstr, parent);
			layout->insertWidget(layout->count() - 1, qc->myWidget());
			return qc;
		}
	case 6:
		if (initstr == "addqty")
		{
			QuantityControl* qc = new QuantityControl(initstr, parent);
			layout->insertWidget(layout->count() - 1, qc->myWidget());
			return qc;
		}
	}
	return Q_NULLPTR;
}
