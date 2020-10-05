#pragma once
#include <QString>



class AbsPrinterWrapper;
class QObject;
namespace PrinterWrapperFactory
{
	AbsPrinterWrapper* fabricate(QObject* parent, QString encoding = QStringLiteral("CP1251"));
}