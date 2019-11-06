#pragma once
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"

/*
	This widget provides intSpinbox wrapped in abs_control interface

*/

class QuantityControl :public abs_control
{
protected:
	// Inherited via abs_control
	BigButtonsSpinbox* innerSpinbox;
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual bool hasFocus() const override;
public:
	explicit QuantityControl(QString& cname, QWidget* parent);
	explicit QuantityControl(QString& cname, QString& assocBuffer, QWidget* parent);
	virtual ~QuantityControl() override;

	// Inherited via abs_control
	virtual void setFocus() const override;

	// Inherited via abs_control
	virtual void show() override;

	virtual void hide() override;

	// Inherited via abs_control
	virtual void refresh() override;

	// Inherited via abs_control
	virtual void installEventFilter(QObject* keyfilter) override;
	virtual QWidget* myWidget() override { return innerSpinbox; };
};
