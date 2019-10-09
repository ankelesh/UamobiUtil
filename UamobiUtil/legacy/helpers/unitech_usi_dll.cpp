#ifdef FTR_INPUT_USIDLL

#include <QLibrary>

#include "unitech_usi_dll.h"

#include <qt_windows.h>

class UsiDllHelper : private QWidget
{
	UsiDllInputHelper* m_method;
	const unsigned m_msgid;

	UsiDllHelper();
	~UsiDllHelper();

protected:
	bool winEvent(MSG* message, long* result);

public:
	bool available();
	static UsiDllHelper* instance();
	void setReceiver(UsiDllInputHelper* m);

	typedef BOOL(*USI_Register_t)(HWND, UINT);
	typedef void (*USI_Unregister_t)(void);
	typedef BOOL(*USI_EnableScan_t)(BOOL);
	typedef UINT(*USI_GetData_t)(LPBYTE, UINT, UINT*);

	static USI_Register_t   USI_Register;
	static USI_Unregister_t USI_Unregister;
	static USI_EnableScan_t USI_EnableScan;
	static USI_GetData_t    USI_GetData;

	static const int SM_DATAREADY = 5;
};

UsiDllHelper::USI_Register_t   UsiDllHelper::USI_Register = 0;
UsiDllHelper::USI_Unregister_t UsiDllHelper::USI_Unregister = 0;
UsiDllHelper::USI_EnableScan_t UsiDllHelper::USI_EnableScan = 0;
UsiDllHelper::USI_GetData_t    UsiDllHelper::USI_GetData = 0;

UsiDllHelper::UsiDllHelper()
	: m_method(0), m_msgid(WM_USER + 42), QWidget(0)
{
	available();
}

UsiDllHelper::~UsiDllHelper()
{
	if (available())
		USI_Unregister();
}

UsiDllHelper* UsiDllHelper::instance()
{
	static UsiDllHelper* i = new UsiDllHelper;
	return i;
}

void UsiDllHelper::setReceiver(UsiDllInputHelper* m)
{
	static bool registered = false;
	if (!registered && available()) {
		USI_Register(this->winId(), m_msgid);
		registered = true;
	}
	m_method = m;
}

bool UsiDllHelper::winEvent(MSG* message, long* result)
{
	if (message->message == m_msgid) {
		if (m_method) {
			switch (message->wParam) {
			case SM_DATAREADY: m_method->readData();
			}
		}
		*result = 0;
		return true;
	}
	return false;
}

bool UsiDllHelper::available()
{
	static bool resolveDone = false;
	if (!resolveDone) {
		QLibrary usi("USI.dll");
		USI_Register = static_cast<USI_Register_t>  (usi.resolve("USI_Register"));
		USI_Unregister = static_cast<USI_Unregister_t>(usi.resolve("USI_Unregister"));
		USI_EnableScan = static_cast<USI_EnableScan_t>(usi.resolve("USI_EnableScan"));
		USI_GetData = static_cast<USI_GetData_t>   (usi.resolve("USI_GetData"));
	}
	return USI_Register && USI_Unregister && USI_EnableScan && USI_GetData;
}

// ---

UsiDllInputHelper::UsiDllInputHelper(QObject* p)
	: InputHelper(p)
{
	UsiDllHelper::instance()->setReceiver(this);
}

UsiDllInputHelper::~UsiDllInputHelper()
{
}

bool UsiDllInputHelper::run()
{
	return true;
}

void UsiDllInputHelper::readData()
{
	if (!UsiDllHelper::instance()->available())
		return;

	unsigned char ubuffer[1024] = { 0 };
	char buffer[1024] = { 0 };
	unsigned type;
	int len = UsiDllHelper::USI_GetData(ubuffer, sizeof buffer - 1, &type);
	buffer[len] = 0;
	memcpy(buffer, ubuffer, sizeof buffer);
	QString code = buffer;
	if (code.at(0) == '$')
		code.remove(0, 1);
	code = code.trimmed(); // USI.dll appends space to the end
	emit newCode(code);
}

#endif // FTR_INPUT_USIDLL