#pragma once
#include <qobject.h>





class ControlTranslator : private QObject
{
	Q_OBJECT
public:
	ControlTranslator();
	QString translate(QString&);
};


QString makeControlTranslation(QString&);