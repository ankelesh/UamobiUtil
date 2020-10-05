#pragma once
#include "AbsPrinterWrapper.h"

class QFile;
class NullPrinterWrapper : public AbsPrinterWrapper
{
	Q_OBJECT
protected:
	QFile* fdest;

	// Inherited via AbsPrinterWrapper
	virtual bool _isValid() const override;
	virtual bool _isReady() const override;
	virtual void _retryOpeningConnection() override;
	virtual QString _errors() const override;
	virtual void _print(QString data) override;
	virtual void _establishConnection() override;

public:
	NullPrinterWrapper(QString filepath, QObject* parent = Q_NULLPTR, QString encoding = QStringLiteral("CP1251"));



};