#include "QuantityControl.h"

QString QuantityControl::prepareAndReturnValue() const
{
	return QString::number(innerSpinBox->value());
}

bool QuantityControl::parseAndSetValue(QString str)
{
	bool ok;
	int toSet = str.toInt(&ok);
	if (ok)
	{
		innerSpinBox->setValue(toSet);
		return true;
	}
	return false;
}

void QuantityControl::clear()
{
	innerSpinBox->setValue(0);
}

bool QuantityControl::valueAvailable() const
{
	return innerSpinBox->value() > 0;
}

QuantityControl::QuantityControl(QWidget* parent)
	: abs_control(Quantity, parent),
	innerSpinBox(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this))
{
	mainLayout->addWidget(innerSpinBox);
	innerSpinBox->setMinimum(0);
	innerSpinBox->setMaximum(99999);
	innerSpinBox->setValue(0);
	hide();
}
