#include <QTimer>
#include <QDebug>

#include "helpermanager.h"

#ifdef FTR_INPUT_USIDLL
#include "unitech_usi_dll.h"
#endif

#ifdef FTR_INPUT_DLSCANNER
#include "datalogic_dlscanner.h"
#endif

#include "emulatedkbdinputhelper.h"

HelperManager::HelperManager(QObject *parent) :
	QObject(parent), m_enabled(false)
{
#ifdef FTR_INPUT_USIDLL
	UsiDllHelper *usiDllHelper = new UsiDllHelper(this);
	if (usiDllHelper && usiDllHelper->run()) {
		connect(usiDllHelper, SIGNAL(newCode(QString)), SLOT(onNewCode(QString)));
	} else {
		delete usiDllHelper;
	}
#endif // FTR_INPUT_USIDLL

#ifdef FTR_INPUT_DLSCANNER
	DLScannerInputHelper *dlsHelper = new DLScannerInputHelper(this);
	if (dlsHelper && dlsHelper->run()) {
		connect(dlsHelper, SIGNAL(newCode(QString)), SLOT(onNewCode(QString)));
	} else {
		delete dlsHelper;
	}
#endif // FTR_INPUT_DLSCANNER

    EmulatedKbdInputHelper *kbdHelper = new EmulatedKbdInputHelper(this);
    connect(kbdHelper, SIGNAL(newCode(QString)), SLOT(onNewCode(QString)));
    kbdHelper->run();
}

HelperManager *HelperManager::instance()
{
	static HelperManager _instance;
    return &_instance;
}

void HelperManager::enable()
{
	m_enabled = true;
}

void HelperManager::disable()
{
	m_enabled = false;
}

void HelperManager::onNewCode(const QString &code)
{
    if (m_enabled){
		emit newCode(code);
    }
}
