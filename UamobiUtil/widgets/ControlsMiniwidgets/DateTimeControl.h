#pragma once
#include "abs_control.h"
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"




class DateTimeControl : public abs_control
{
	Q_OBJECT
private:
	BigButtonsSpinbox* innerSpinbox;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual QWidget* getInnerWidget() const override;
public:
	explicit DateTimeControl();
	explicit DateTimeControl(bool needsTime, QString cname=QString(), QWidget* parent = Q_NULLPTR);
	virtual ~DateTimeControl();
	virtual void setFocus() const override;
	virtual void show() override;
	virtual void hide() override;

};


