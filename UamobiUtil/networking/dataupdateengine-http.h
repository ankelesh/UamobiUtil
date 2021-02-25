#pragma once
#include <QNetworkRequest>
#include <QNetworkReply>
#include "networking/RequestAwaiter.h"
#include "QueryTemplates.h"
#include "submodules/UNAQtCommons/consts/DatetimeConstants.h"

/*
	This file contains singletone class httpupdateengine, which provides common wrapper 
	around http interfaces. It stores url used for connection and some system variables.
	Generally it does not executes plain http requests to avoid hardcoding, instead of this
	it supports both templated queries and overloadable queries. Templated queries are always faster
	but less adaptable. If you need to track your query response, you can provide Awaiter object, which 
	is wrapping NetworkReply as adapter. 
*/



#define AppNetwork HttpUpdateEngine::instanse()
using namespace QueryTemplates;
class HttpUpdateEngine : public QObject
{
	Q_OBJECT
private:
	// url to use as query "prefix"
	QString url;
	// query id is required for backend, it must grow on every query
	unsigned int nextQueryId;
	// delay is deprecated variable
	qint64 delay;
	// session id is required for most API interactions, it is acquired via login
	QString sessionId;
	// network access manager tracks all sends and responces
	QNetworkAccessManager netManager;

	static HttpUpdateEngine* _instanse;

	// this method sends plain url. It is hidden to avoid hardcoding.
	void sendQuery
	(
		const QString& urlpath,
		RequestAwaiter* awaiter
	);
public:
	HttpUpdateEngine(QString& Url, QObject* parent);
	// performs query without attaching session to it
	void execQueryOutsideSession(QueryTemplates::QueryId id, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	// inserts query id and session into templated url, then calls sendQuery
	void execQueryByTemplate(QueryTemplates::QueryId id, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, int argc, QStringList argv, RequestAwaiter* awaiter);
	// inserts query id and session into url provided by overloadable query, then applies arguments to it
	void execQueryByTemplate(const OverloadableQuery& oq, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, int argc, QStringList argv, RequestAwaiter* awaiter);

	// resets connection and sends ping to new server destination
	void setUrl(QString url) ;
	void clearSession();
	// obtains session id from login response
	QString setSession(QString& requestResult);
	// simply returns url 
	QString getUrl();
	// checks session
	bool sessionReady();
	// singletone interface
	static HttpUpdateEngine* instanse();
};

// these formats are used to encode dates to avoid server incompatibility
extern const QString PARSER_DATETIME_ENCODING_FORMAT;
