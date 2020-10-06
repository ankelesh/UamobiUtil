#pragma once
#include "AbsPrinterWrapper.h"
#ifdef Q_OS_ANDROID
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#endif

class QBluetoothSocket;
class QBluetoothLocalDevice;
class QBluetoothAddress;
class QBluetoothServiceDiscoveryAgent;
class AndroidBluetoothPrinterWrapper : public AbsPrinterWrapper
{
	Q_OBJECT
protected:
#ifdef Q_OS_ANDROID
	QBluetoothLocalDevice* localDevice;
	QBluetoothServiceInfo targetService;
	QBluetoothServiceDiscoveryAgent* serviceDiscAgent;
	QBluetoothSocket* mainSocket;
#endif
	enum ConnectionMode {INVALID, LAST, NEWCONN};
	QString targetDeviceName;
    bool blocker;
	ConnectionMode connectionMode;
#ifdef Q_OS_ANDROID
	QBluetoothAddress lastMAC;
	QBluetoothUuid lastUUID;
#endif
	// Inherited via AbsPrinterWrapper
	virtual bool _isValid() const override;
	virtual bool _isReady() const override;
	virtual void _retryOpeningConnection() override;
	virtual QString _errors() const override;
	virtual void _print(QString data) override;
	virtual void _establishConnection() override;
	void _clearAndLaunchDiscovery();
	void _openConnection();
public:
	AndroidBluetoothPrinterWrapper(QString device_name, QObject* parent = Q_NULLPTR, QString encoding = QStringLiteral("CP1251"));
	AndroidBluetoothPrinterWrapper(QString device_name, QString lmac, QString luuid, QObject* parent = Q_NULLPTR, QString encoding = QStringLiteral("CP1251"));
	virtual ~AndroidBluetoothPrinterWrapper()
#ifdef QT_VERSION5X
		override
#endif
		;
#ifdef Q_OS_ANDROID
protected slots:
	void serviceFound(const QBluetoothServiceInfo& svc);
	void socketReady();
	void connectionError(QBluetoothSocket::SocketError err);
#endif

};
