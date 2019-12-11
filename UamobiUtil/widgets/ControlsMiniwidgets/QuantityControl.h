#pragma once
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "widgets/ControlsMiniwidgets/abs_control.h"

/*
	This widget provides numeric Spinbox wrapped in abs_control interface.
	You can provide isInt parameter to define which precision should be used
	in spinbox. Default (and way slower) is double. Spinbox will update associated
	buffer with new values if user is modifying it directly. You can make connection
	between controls, which will allow autojump on editingFinished. Do not forget
	to provide control name in original notation - you should not translate it.

*/

class QuantityControl :public abs_control
{
protected:
	BigButtonsSpinbox* innerSpinbox;		//	Polymorthic spinbox
	control_utils::refresher* synchronizer;
	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual bool hasFocus() const override;
    virtual void setListening(bool) override;
public:
	explicit QuantityControl(bool isInt, QString& cname, QWidget* parent);
	explicit QuantityControl(bool isInt, QString& cname, QString& assocBuffer, QWidget* parent);
    virtual ~QuantityControl();			//	must be here, because this is NOT QObject

	// outer interface functions
	virtual void setFocus() const override;
	virtual void show() override;
	virtual void hide() override;
	virtual void refresh() override;
	virtual void installEventFilter(QObject* keyfilter) override;
	virtual QWidget* myWidget() override { return innerSpinbox; };
	virtual void makeConnectionBetweenControls(abs_control* another) override;
};
