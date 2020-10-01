#pragma once
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include <QBoxLayout>
#include <QLabel>
#include "widgets/ControlsMiniwidgets/abs_control.h"

/*
	This widget is used for future pages with multiple controls.
	Now it is not used. Do not include this under Qt4
*/


class ControlListWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QVector<abs_control*> controls;
	QHBoxLayout* footerLayout;
	MegaIconButton* okButton;
	MegaIconButton* backButton;
	virtual void focusInEvent(QFocusEvent* fev) override;
public:
	ControlListWidget(QWidget* parent);
	void clearControls();
    void useControls(const IControlList & controls);
	QStringList getControlsValues();
	QString getValueAt(int index);
	void emplaceControl(InputControl);
	QString joinedControls();
	QString mappedControls();
	~ControlListWidget();
private slots:
	void checkAndConfirmControls();
signals:
	void controlsConfirmed();

};
