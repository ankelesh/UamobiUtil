#pragma once
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"

/*
	This widget provides intSpinbox wrapped in abs_control interface
	

*/

class QuantityControl : public abs_control
{
protected:
	BigButtonsSpinbox* innerSpinBox;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual bool hasFocus() const override;
public:
	explicit QuantityControl(QWidget * parent);

};

