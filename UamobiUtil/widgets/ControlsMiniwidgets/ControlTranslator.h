#pragma once
#include <qobject.h>





class ControlTranslator : private QObject
{
public:
	ControlTranslator();
	QString translate(QString&);
};


QString makeControlTranslation(QString&);