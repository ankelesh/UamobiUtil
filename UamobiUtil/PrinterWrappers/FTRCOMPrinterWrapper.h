#pragma once
#include "AbsPrinterWrapper.h"


class TTY;
class FTRCOMPrinterWrapper : public AbsPrinterWrapper
{
	Q_OBJECT
protected:
#ifdef FTR_COM
	TTY* printerSocket;
#endif
	QString socket;
	int baudRate;
	// Inherited via AbsPrinterWrapper
	virtual bool _isValid() const override;
	virtual bool _isReady() const override;
	virtual void _retryOpeningConnection() override;
	virtual QString _errors() const override;
	virtual void _print(QString data) override;
	virtual void _establishConnection() override;

public:
	FTRCOMPrinterWrapper( QString port, int brate, QObject* parent = Q_NULLPTR, QString encoding = QStringLiteral("CP1251"));
	virtual ~FTRCOMPrinterWrapper()
#ifdef QT_VERSION5X
		override
#endif
		;

};