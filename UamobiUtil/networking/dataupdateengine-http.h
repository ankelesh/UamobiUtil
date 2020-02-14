#ifndef DATAUPDATEENGINEHTTP_H
#define DATAUPDATEENGINEHTTP_H
#include <QNetworkRequest>
#include <QNetworkReply>
#include "networking/RequestAwaiter.h"
#include "QueryTemplates.h"


#define AppNetwork HttpUpdateEngine::instanse()
using namespace QueryTemplates;
class HttpUpdateEngine : public QObject
{
	Q_OBJECT
private:
	QString url;

	static HttpUpdateEngine* _instanse;
public:
	HttpUpdateEngine(QString& Url, QObject* parent);
	void execQueryOutsideSession(QueryTemplates::QueryId id, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId id, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(QueryTemplates::QueryId  id, int argc, QStringList argv, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(const OverloadableQuery& oq, int argc, QStringList argv, RequestAwaiter* awaiter);

	void setUrl(QString url) ;
	QString setSession(QString& requestResult);
	QString getUrl();
	bool sessionReady();
	static HttpUpdateEngine* instanse();
private:
	void sendQuery
	(
		const QString& urlpath,
		RequestAwaiter* awaiter
	);
	unsigned int nextQueryId;
	qint64 delay;
	QString sessionId;
	QNetworkAccessManager netManager;
};

extern const QString DATE_ENCODING_FORMAT;
QString makeParamsFromList(QVector<QPair<QString, QString> >& vect);
QString makeParamsFromList(QString& paramName, QString& paramVal);
#endif // DATAUPDATEENGINEHTTP_H
