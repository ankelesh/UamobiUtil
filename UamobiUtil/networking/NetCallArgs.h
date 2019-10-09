#pragma once
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QDate>
#include <QtCore/QStringList>

/*
	This file is containing structure for arguments used in network queries. It contains lot of constructors to fill
	inner lists and outer function checkNetCallArgsQuantity to verify if arguments can be used in particular query.

	main idea of this structure is to unify interfaces of all network queries - this is usefull for reusing code.
	When all functions has similar signatures, they can be easily swapped in pointers, this leads to easy
	changes in widgets behaviour - instead of hardcoding network queries we can provide to widget constructor a pointer to function.

	Warning! When all functions has the same signatures, it is easy to misplace one. This will lead to skipping queries. If possible, unite
	functions under one specific signature instead of using NCA. NCA should be used in case when two functions has REALLY different signatures
	like (void) -> (int,int,int,int).

	Usefull typo:
		try to make your signatures compatible without using NCA. NCA is usefull thing when

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
	NetCallArgs(QString&, QString = "");
	NetCallArgs(bool, QString&);
	NetCallArgs();
	NetCallArgs(QStringList*, QList<int>* = Q_NULLPTR, QList<double>* = Q_NULLPTR, QList<bool>* = Q_NULLPTR, QList<QDate>* = Q_NULLPTR);
};
bool checkNetCallArgsQuantity(NetCallArgs* nca, int sargs = 0, int iargs = 0, int dargs = 0, int bargs = 0, int daargs = 0);
