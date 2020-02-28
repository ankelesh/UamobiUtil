#include <Qt>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#ifdef FTR_COM
#include "tty.h"
static int TIMEOUT = 3000;

TTY::TTY() : Handle(0), isReady(false) {
}

TTY::~TTY() {
	disconnect();
}

void TTY::connect(const QString &port, int baudrate)
{
#ifdef DEBUG
	detrace_METHCALL("connect");
#endif
	disconnect();

	wchar_t *buff = new wchar_t[port.length()+1];
	memset(buff, 0, port.length()+1);
	buff[port.toWCharArray(buff)] = 0;

	Handle =
			CreateFile(
				buff,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
	delete[] buff;

	if (Handle == INVALID_HANDLE_VALUE) {
		Handle = 0;
#ifdef DEBUG
		detrace_METHPERROR("", "CreateFile failed");
#endif
		errorBuffer += "Create file failed";
		return;
	}

	SetCommMask(Handle, EV_RXCHAR);
	SetupComm(Handle, 1500, 1500);

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;

	if (!SetCommTimeouts(Handle, &CommTimeOuts)) {
		Handle = 0;
#ifdef DEBUG
		detrace_METHPERROR("", "SetCommTimeouts failed");
#endif
		errorBuffer += "SetCommTimeouts failed";
		return;
	}

	DCB ComDCM;

	memset(&ComDCM,0,sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(Handle, &ComDCM);
	ComDCM.BaudRate = DWORD(baudrate);
	ComDCM.ByteSize = 8;
	ComDCM.Parity = NOPARITY;
	ComDCM.StopBits = ONESTOPBIT;
	ComDCM.fAbortOnError = TRUE;
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE;
	ComDCM.fRtsControl = RTS_CONTROL_DISABLE;
	ComDCM.fBinary = TRUE;
	ComDCM.fParity = FALSE;
	ComDCM.fInX = FALSE;
	ComDCM.fOutX = FALSE;
	ComDCM.XonChar = 0;
	ComDCM.XoffChar = (unsigned char)0xFF;
	ComDCM.fErrorChar = FALSE;
	ComDCM.fNull = FALSE;
	ComDCM.fOutxCtsFlow = FALSE;
	ComDCM.fOutxDsrFlow = FALSE;
	ComDCM.XonLim = 128;
	ComDCM.XoffLim = 128;

	if (!SetCommState(Handle, &ComDCM)) {
		CloseHandle(Handle);
		Handle = 0;
#ifdef DEBUG
		detrace_METHEXPL("SetCommState failed");
#endif
		errorBuffer += "SetCommState failed";
		return;
	}
	isReady = true;
}

void TTY::disconnect() {

	if (Handle != 0) {
		CloseHandle(Handle);
		Handle = 0;
	}
	isReady = false;
	errorBuffer.clear();
}

bool TTY::write(const QByteArray &data)
{
	if (Handle == 0) {
		errorBuffer += "Connection is closed";
		isReady = false;
		return false;
	}

	DWORD feedback;
	if (!WriteFile(Handle, data.data(), data.size(), &feedback, 0) || feedback != (DWORD)data.size()) {
		CloseHandle(Handle);
		Handle = 0;
#ifdef DEBUG
		detrace_METHEXPL("WriteFile failed");
#endif
		errorBuffer += "WriteFile failed";
		isReady = false;
		return false;
	}
	return true;
	// In some cases it's worth uncommenting
	//FlushFileBuffers(Handle);

}

void TTY::read(QByteArray data)
{

	if (Handle == 0) {
		return;
	}

	DWORD begin = GetTickCount();
	DWORD feedback = 0;

	char* buf = data.data();
	DWORD len = (DWORD)data.size();

	int attempts = 3;
	while(len && (attempts || (GetTickCount()-begin) < (DWORD)TIMEOUT/3)) {

		if (attempts) attempts--;

		if (!ReadFile(Handle, buf, len, &feedback, NULL)) {
			CloseHandle(Handle);
			Handle = 0;
			return;
		}

		Q_ASSERT(feedback <= len);
		len -= feedback;
		buf += feedback;

	}

	if (len) {
		CloseHandle(Handle);
		Handle = 0;
		return;
	}

}

#endif // FTR_COM
