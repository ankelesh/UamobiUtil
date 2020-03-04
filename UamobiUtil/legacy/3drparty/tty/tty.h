#ifndef TTY_H
#define TTY_H
#ifdef FTR_COM
#include <QString>
#include <qt_windows.h>

class TTY {
	HANDLE Handle;
	bool isReady;
	QString errorBuffer;

public:
	TTY();
	void connect(const QString &port, int baudrate);
	void disconnect();
	bool write(const QByteArray &data);
	void read(QByteArray data);
    bool isConnected() { return isReady; }
	QString getErrors() {
        return errorBuffer;
    }
	~TTY();
};

#endif
#endif
