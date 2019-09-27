#ifndef DATALOGIC_DLSCANNER_H
#define DATALOGIC_DLSCANNER_H


//LEGACY
#include "abstractinputhelper.h"

class DLScannerInputHelper : public AbstractInputHelper
{
public:
	explicit DLScannerInputHelper(QObject *parent = 0);
	~DLScannerInputHelper();
	void emitNewCode(const QString &c);
	bool enable();
	bool disable();
	virtual bool run();
};

#endif // DATALOGIC_DLSCANNER_H
