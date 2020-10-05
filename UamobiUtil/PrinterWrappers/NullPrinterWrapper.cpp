#include "NullPrinterWrapper.h"
#include <QFile>
bool NullPrinterWrapper::_isValid() const
{
	return true;
}

bool NullPrinterWrapper::_isReady() const
{
	return fdest->isOpen();
}

void NullPrinterWrapper::_retryOpeningConnection()
{
	fdest->open(QIODevice::WriteOnly);
	if (!fdest->isOpen())
	{
		errorOutput = tr("file invalid");
		emit error(errorOutput);
	}
	else
		emit connected();
}

QString NullPrinterWrapper::_errors() const
{
	return errorOutput;
}

void NullPrinterWrapper::_print(QString data)
{
	fdest->write(encoder.fromUnicode(data));
	fdest->flush();
}

NullPrinterWrapper::NullPrinterWrapper(QString filepath, QObject* parent, QString encoding)
	: AbsPrinterWrapper(None, parent, encoding), fdest(new QFile(filepath, this))
{
	
}

void NullPrinterWrapper::_establishConnection()
{
	fdest->open(QIODevice::WriteOnly);
	if (!fdest->isOpen())
	{
		errorOutput = tr("file invalid");
		emit error(errorOutput);
	}
	else
		emit connected();
}
