#pragma once
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"


class QuantityControl : public abs_control
{
protected:
	BigButtonsSpinbox* innerSpinBox;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;

public:
	explicit QuantityControl(QWidget * parent);

};

