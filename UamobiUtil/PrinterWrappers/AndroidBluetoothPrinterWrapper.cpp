#include "AndroidBluetoothPrinterWrapper.h"

#ifdef Q_OS_ANDROID
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>
#endif
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/GlobalAppSettings.h"
bool AndroidBluetoothPrinterWrapper::_isValid() const
{
#ifdef Q_OS_ANDROID
    return true;
#else
	return false;
#endif
}

bool AndroidBluetoothPrinterWrapper::_isReady() const
{
#ifdef Q_OS_ANDROID
	return mainSocket->state() == QBluetoothSocket::ConnectedState;
#else
    return false;
#endif
}

void AndroidBluetoothPrinterWrapper::_retryOpeningConnection()
{
    _clearAndLaunchDiscovery();
}

QString AndroidBluetoothPrinterWrapper::_errors() const
{
	return errorOutput;
}

void AndroidBluetoothPrinterWrapper::_print(QString data)
{
#ifdef Q_OS_ANDROID
    if (!isReady())
    {
        errorOutput += tr("connection not ready");
        emit error(errorOutput);
        return;
    }
    mainSocket->write(encoder.fromUnicode(data));
#else
#ifdef DEBUG
    detrace_METHPERROR("_print", "attempt to print without a/b support data "  << data);
#endif
#endif
}
void AndroidBluetoothPrinterWrapper::_clearAndLaunchDiscovery()
{
#ifdef Q_OS_ANDROID
    mainSocket->deleteLater();
    mainSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    QObject::connect(mainSocket, &QBluetoothSocket::connected, this, &AndroidBluetoothPrinterWrapper::socketReady);
    QObject::connect(mainSocket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error), 
        this, &AndroidBluetoothPrinterWrapper::connectionError);

    serviceDiscAgent->clear();
    blocker = false;
    serviceDiscAgent->start();
#endif
}

void AndroidBluetoothPrinterWrapper::_openConnection()
{
#ifdef Q_OS_ANDROID
    QBluetoothLocalDevice::Pairing p = localDevice->pairingStatus(targetService.device().address());
    if (p != QBluetoothLocalDevice::Paired)
    {
        errorOutput = tr("Device not paired!");
        emit error(errorOutput);
        if (connectionMode == LAST)
        {
            connectionMode = NEWCONN;
            _clearAndLaunchDiscovery();
        }
        return;
    }
#ifdef DEBUG
    detrace_METHEXPL("trying to connect to: " << targetService.device().address().toString());
#endif
    mainSocket->connectToService(targetService, QIODevice::WriteOnly);
#endif
}

AndroidBluetoothPrinterWrapper::AndroidBluetoothPrinterWrapper(QString device_name, QObject* parent, QString enc)
	: AbsPrinterWrapper(AndroidBluetooth, parent, enc),
#ifdef Q_OS_ANDROID
	localDevice(new QBluetoothLocalDevice(this)),
	targetService(),
	serviceDiscAgent(new QBluetoothServiceDiscoveryAgent(this)),
	mainSocket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
#endif
    targetDeviceName(device_name),
    blocker(false),
    connectionMode(NEWCONN)
#ifdef Q_OS_ANDROID
    , lastMAC(),
    lastUUID()
#endif
{

}

AndroidBluetoothPrinterWrapper::AndroidBluetoothPrinterWrapper(QString device_name, QString lmac, QString luuid, QObject* parent, QString encoding)
    : AbsPrinterWrapper(AndroidBluetooth, parent, encoding),
#ifdef Q_OS_ANDROID
    localDevice(new QBluetoothLocalDevice(this)),
    targetService(),
    serviceDiscAgent(new QBluetoothServiceDiscoveryAgent(this)),
    mainSocket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
#endif
    targetDeviceName(device_name),
    blocker(false),
    connectionMode(LAST)
#ifdef Q_OS_ANDROID
    ,lastMAC(lmac),
    lastUUID(luuid)
#endif
{
#ifdef Q_OS_ANDROID
    if (lmac.isEmpty() || luuid.isEmpty())
        connectionMode = NEWCONN;
#endif
}

AndroidBluetoothPrinterWrapper::~AndroidBluetoothPrinterWrapper()
{
#ifdef Q_OS_ANDROID
    if (mainSocket->state() == QBluetoothSocket::ConnectedState)
    {
        AppSettings->lastPrinterBTMAC = targetService.device().address().toString();
        AppSettings->lastPrinterBTUUID = targetService.serviceUuid().toString();
    }
    mainSocket->close();
    mainSocket->disconnectFromService();
#endif
}
#ifdef Q_OS_ANDROID
void AndroidBluetoothPrinterWrapper::serviceFound(const QBluetoothServiceInfo& svc)
{
    if (blocker)
        return;
#ifdef DEBUG
        detrace_METHEXPL("FOUND SERVICE: " << svc.serviceName() <<
            " name " << svc.device().name() << " " << svc.serviceUuid().toString());
#endif
    if (svc.device().name().contains(targetDeviceName))
    {
        targetService = svc;
        blocker = true;

        _openConnection();
    }
}
void AndroidBluetoothPrinterWrapper::socketReady()
{
#ifdef DEBUG
    detrace_METHEXPL("connected to: " << targetService.serviceName() <<
        " name " << targetService.device().name() << " " << targetService.serviceUuid().toString());
#endif
    emit connected();
}

void AndroidBluetoothPrinterWrapper::connectionError(QBluetoothSocket::SocketError err)
{
#ifdef DEBUG
    detrace_METHPERROR("AndroidBluetoothPrinterWrappper::_openConnection", "error code: " << err);
#endif
    errorOutput = tr("error opening connection with code:") + QString::number(err);
    emit error(errorOutput);
    if (connectionMode == LAST)
    {
        connectionMode = NEWCONN;
        _clearAndLaunchDiscovery();

    }
}

#endif
void AndroidBluetoothPrinterWrapper::_establishConnection()
{
#ifdef Q_OS_ANDROID
    if (localDevice->isValid())
    {
        localDevice->powerOn();
#ifdef DEBUG
        detrace_METHEXPL("Succesfully started on device named " << localDevice->name());
#endif
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        QObject::connect(serviceDiscAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &AndroidBluetoothPrinterWrapper::serviceFound);
        QObject::connect(mainSocket, &QBluetoothSocket::connected, this, &AndroidBluetoothPrinterWrapper::socketReady);
        QObject::connect(mainSocket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error), this, &AndroidBluetoothPrinterWrapper::connectionError);
        switch (connectionMode)
        {
        case LAST:
            targetService.setDevice(QBluetoothDeviceInfo(lastMAC, targetDeviceName, QBluetoothDeviceInfo::ServiceClass::ObjectTransferService));
            targetService.setServiceUuid(lastUUID);
            _openConnection();
            break;
        case NEWCONN:
            serviceDiscAgent->start();
            break;
        default: break;
        }
        
    }
    else
    {
        errorOutput = tr("device error - no bluetooth support");
        emit error(errorOutput);
#ifdef DEBUG
        detrace_METHPERROR("AndroidBluetoothPrinterWrappper", "device is not valid");
#endif
    }
#else
    errorOutput = tr("no a/bluetooth support on this os");
    emit error(errorOutput);
#ifdef DEBUG
    detrace_METHPERROR("AndroidBluetoothPrinterWrappper", "wrong OS");
#endif
#endif
}
