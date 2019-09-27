#ifndef UNITECH_USI_DLL_H
#define UNITECH_USI_DLL_H


// LEGACY
#include "abstractinputhelper.h"

class UsiDllInputHelper : public AbstractInputHelper
{
public:
	explicit UsiDllInputHelper(QObject *parent = 0);
	~UsiDllInputHelper();
	void readData();

protected:
	virtual bool run();
};

#endif // UNITECH_USI_DLL_H
