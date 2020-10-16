#pragma once
#include <QObject>
#include <QTextEncoder>
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromLatin1(""A"",sizeof(A)-1)
#endif

class AbsPrinterWrapper : public QObject
{
	Q_OBJECT
public:
	enum PrinterConnectionTypes { None, CEComport, AndroidBluetooth };
protected:
	PrinterConnectionTypes type;
	QString errorOutput;
	QTextEncoder encoder;
	virtual bool _isValid() const = 0;
	virtual bool _isReady() const = 0;
	virtual void _retryOpeningConnection() = 0; 
	virtual QString _errors() const = 0;
	virtual void _print(QString data) = 0;
	virtual void _establishConnection() = 0;
public:
	AbsPrinterWrapper(PrinterConnectionTypes ptype = None, QObject* parent = Q_NULLPTR, QString encoding = 
		QStringLiteral("CP1251"));
	bool isValid() const;
	bool isReady() const;
	void retryOpeningConnection();
	void establishConnection();
	QString errors() const;
	virtual ~AbsPrinterWrapper();
	
public slots:
	void print(QString data);
signals:
	void error(QString errorString);
	void connected();
};
