#ifdef FTR_INPUT_DLSCANNER

#include "datalogic_dlscanner.h"
#include <qt_windows.h>

#ifndef SEDAPI_API
#	define SEDAPI_API __declspec(dllimport)
#endif
#ifndef DL_TCHAR
#	define DL_TCHAR TCHAR
#endif

#include <DLScanner.h>

class DLScannerHelper : private QWidget
{
public:
	static DLScannerHelper* instance();
	void setReceiver(DLScannerInputHelper* rec);
	void enable();
	void disable();

protected:
	bool winEvent(MSG* message, long* result);

private:
	DLScanner m_scanner;
	DLScannerInputHelper* m_receiver;
	const unsigned long WM_DLScannerMessage;

	DLScannerHelper();
	~DLScannerHelper();
};

DLScannerHelper* DLScannerHelper::instance()
{
	static DLScannerHelper i;
	return &i;
}

void DLScannerHelper::setReceiver(DLScannerInputHelper* rec)
{
	m_receiver = rec;
}

void DLScannerHelper::enable()
{
	m_scanner.scanEnable();
}

void DLScannerHelper::disable()
{
	m_scanner.scanDisable();
}

bool DLScannerHelper::winEvent(MSG* message, long* result)
{
	if (message->message == WM_DLScannerMessage) {
		*result = 0;
		const int len = m_scanner.getLabelTextLength();
		wchar_t* label = new wchar_t[len + 1];
		memset(label, 0, (len + 1) * sizeof(wchar_t));
		m_scanner.getLabelText(label, len);
		if (m_receiver)
			m_receiver->emitNewCode(QString::fromUtf16(label, len));
		delete[] label;
		return true;
	}
	return false;
}

DLScannerHelper::DLScannerHelper() :
	WM_DLScannerMessage(WM_USER + 42)
{
	m_scanner.registerLabelMessage(this->window()->winId(), WM_DLScannerMessage);
}

DLScannerHelper::~DLScannerHelper()
{
	m_scanner.deregisterLabelMessage();
}

DLScannerInputHelper::DLScannerInputHelper(QObject* parent) :
	AbstractInputHelper(parent)
{
	DLScannerHelper::instance()->setReceiver(this);
}

DLScannerInputHelper::~DLScannerInputHelper()
{
	DLScannerHelper::instance()->disable();
	DLScannerHelper::instance()->setReceiver(0);
}

void DLScannerInputHelper::emitNewCode(const QString& c)
{
	QString code = c.trimmed();
	if (code.startsWith('$'))
		code = code.right(code.length() - 1);
	if (code.endsWith('$'))
		code = code.left(code.length() - 1);
	emit newCode(code);
}

bool DLScannerInputHelper::enable()
{
	DLScannerHelper::instance()->enable();
	return true;
}

bool DLScannerInputHelper::disable()
{
	DLScannerHelper::instance()->disable();
	return true;
}

bool DLScannerInputHelper::run()
{
	return true;
}

#endif // FTR_INPUT_DLSCANNER