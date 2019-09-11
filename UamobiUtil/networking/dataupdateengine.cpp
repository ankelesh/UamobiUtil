#include "dataupdateengine.h"
#include "dataupdateengine-http.h"
#include "debugtrace.h"

DataUpdateEngine::DataUpdateEngine(QObject *parent) :
	QObject(parent)
{
    //detrace_DCONSTR("DataUpdateEngine")
}

DataUpdateEngine* DataUpdateEngine::byName(QString name, QObject *parent)
{
   // detrace_METHCALL("DataUpdateEngine::byName")
	name = name.toLower();
#ifdef FTR_DATA_INMEMORY
	if (name == "mem")
		return new InMemEngine(parent);
	else
#endif // FTR_DATA_INMEMORY
		return new HttpUpdateEngine(QString(""), parent);
}

DataUpdateEngine *DataUpdateEngine::getMain(QObject *parent)
{
    //detrace_METHCALL("DataUpdateEngine::getMain")
	//return byName(settings()->value("dataeng", "http").toString(), parent);
	return  byName("dataeng",parent);
}

