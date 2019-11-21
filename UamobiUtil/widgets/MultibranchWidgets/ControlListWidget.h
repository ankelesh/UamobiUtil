#pragma once
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>




class ControlListWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* userInfo;
	QLabel* barcodeInfo;
	QLabel* addInfo;
	QVector<abs_control*> controls;
	QHBoxLayout* footerLayout;
	MegaIconButton* okButton;
	MegaIconButton* backButton;

public:
	ControlListWidget(QWidget* parent);

	void clearControls();
	void useControls(QStringList & initstrings);
	void useControls(QStringList& initstrings, QStringList& cvalues);
	QStringList getControlsValues();
	QString getValueAt(int index);
	void emplaceControl(QString& initstr);
	void setLabels(QString barcode, QString additional = "", QString uinfo = "");
	void clearLabels();
	~ControlListWidget();
private slots:
	void checkAndConfirmControls();
signals:
	void controlsConfirmed();

};