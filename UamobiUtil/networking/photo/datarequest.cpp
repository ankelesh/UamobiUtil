#include "datarequest.h"
#include "../dataupdateengine.h"
#include "debugtrace.h"

int DataRequest::activeRequestsCount = 0;

DataRequest::DataRequest(Request rt, QObject* parent) :
	QObject(parent), m_requestType(rt)
{
#ifdef DEBUG
	//detrace_DCONSTR("dataRequest");
#endif
}

DataRequest::Request DataRequest::type()
{
	return m_requestType;
}

bool DataRequest::hasErrors()
{
	return !m_errMsg.isEmpty();
}

QString DataRequest::errorMessage()
{
	return m_errMsg;
}

void DataRequest::setErrorMessage(const QString& str)
{
	m_errMsg = str;
}

QVariant DataRequest::data()
{
	return m_resultData;
}

void DataRequest::setData(QVariant& data)
{
	m_resultData = data;
}

void DataRequest::call(DataUpdateEngine* e)
{
	if (!e)
		return;
	e->makeRequest(this);
	activeRequestsCount++;
}

void DataRequest::setDone()
{
	activeRequestsCount--;
	emit done(this);
}

void DataRequest::setRequestParam(const char* name, const QVariant& value)
{
	setProperty(name, value);
}

QVariant DataRequest::requestParam(const char* name) const
{
	return property(name);
}