#pragma once
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"

/*
	This widget provides numeric Spinbox wrapped in abs_control interface.
	You can provide isInt parameter to define which precision should be used
	in spinbox.  Do not forget
	to provide control name in original notation - you should not translate it.

*/

class QuantityControl :public abs_control
{
protected:
	BigButtonsSpinbox* innerSpinbox;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual QWidget* getInnerWidget() const override;
public:
	explicit QuantityControl(bool isInt, QString& cname, QWidget* parent);
	virtual void show() override;
	virtual void hide() override;
	virtual void setFocus() const override;
    virtual ~QuantityControl();
};
