#include "NetCallArgs.h"

NetCallArgs::NetCallArgs(bool barg)
{
	boolArgs.push_back(barg);
}

NetCallArgs::NetCallArgs(QDate& dat)
{
	dateArgs.push_back(dat);
}

NetCallArgs::NetCallArgs(QString& fs, QString ss)
{
	stringArgs << fs;
	if (ss != "")
	{
		stringArgs << ss;
	}
}

NetCallArgs::NetCallArgs()
{
}

NetCallArgs::NetCallArgs(QStringList* sp, QList<int>* ip, QList<double>* dbp , QList<bool>* bp, QList<QDate>* dp)
{
	if (sp != Q_NULLPTR)
	{
		stringArgs << *sp;
	}
	if (ip != Q_NULLPTR)
	{
		intArgs << *ip;
	}
	if (dbp != Q_NULLPTR)
	{
		doubleArgs << *dbp;
	}
	if (bp != Q_NULLPTR)
	{
		boolArgs << *bp;
	}
	if (dp != Q_NULLPTR)
	{
		dateArgs << *dp;
	}
}


NetCallArgs::NetCallArgs(double f, double s)
{
	doubleArgs << f;
	if (s != 0.0)
	{
		doubleArgs << s;
	}
}

bool checkNetCallArgsQuantity(NetCallArgs* nca, int sargs, int iargs, int dargs, int bargs, int daargs)
{
	if (nca == Q_NULLPTR)
		return false;
	if (nca->stringArgs.count() != sargs)
		return false;
	if (nca->intArgs.count() != iargs)
		return false;
	if (nca->doubleArgs.count() != dargs)
		return false;
	if (nca->boolArgs.count() != bargs)
		return false;
	if (nca->dateArgs.count() != daargs)
		return false;
	return true;
}
