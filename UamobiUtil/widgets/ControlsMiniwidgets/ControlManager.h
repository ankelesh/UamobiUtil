#pragma once
#include <QObject>
#include <QHash>
#include "abs_control.h"

/*
	TODO:
		this

*/


class ControlManager : public QObject
{
	Q_OBJECT
private:
	QHash<QString, abs_control*> controlsList;
	QString currentControl;
	bool doesNeedValues;
	int controls;
	QLayout* whereToPlace;

	void allocateIntControl(QString& name);
public:
	ControlManager(QWidget* parent, QLayout* layout);
	bool parseControls(QStringList);
	bool parseControls(QString);
	void setControl(QString);
	void setControl(int);
	void setCurrentValue(QString);
	void setCurrentValue(int);
	
private slots:
	void listenControl(QString&);
public slots:
	void autosetControl(QString&);
signals:
	void controlChanged(QString&);
};