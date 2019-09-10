#pragma once
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QDate>


/*
	This file



*/

struct NetCallArgs
{
	QStringList stringArgs;
	QList<int> intArgs;
	QList<double> doubleArgs;
	QList<bool> boolArgs;
	QList<QDate> dateArgs;
	NetCallArgs(bool);
	NetCallArgs(QDate&);
	NetCallArgs(double, double = 0.0);
	NetCallArgs(QString &, QString = "");
	NetCallArgs();
	NetCallArgs(QStringList*, QList<int>* = Q_NULLPTR, QList<double>* = Q_NULLPTR, QList<bool>* = Q_NULLPTR, QList<QDate>* = Q_NULLPTR);
};
