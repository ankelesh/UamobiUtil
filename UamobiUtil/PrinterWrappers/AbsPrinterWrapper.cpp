#include "AbsPrinterWrapper.h"
#include <QTextEncoder>
#include <qtimer.h>

AbsPrinterWrapper::AbsPrinterWrapper(PrinterConnectionTypes ptype, QObject* parent, QString encoding)
	: QObject(parent), type(ptype), errorOutput(), encoder(QTextCodec::codecForName(encoding.toLatin1()))
{

}

bool AbsPrinterWrapper::isValid() const
{
	return _isValid();
}

bool AbsPrinterWrapper::isReady() const
{
	return _isReady();
}

void AbsPrinterWrapper::retryOpeningConnection()
{
	_retryOpeningConnection();
}

void AbsPrinterWrapper::establishConnection()
{
	_establishConnection();
}

QString AbsPrinterWrapper::errors() const
{
	return _errors();
}

void AbsPrinterWrapper::print(QString data)
{
	_print(data);
}


AbsPrinterWrapper::~AbsPrinterWrapper()
{
}
