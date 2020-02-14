#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml>
#include <QDomElement>
#include <QDomNodeList>
#include <QtCore/QTextDecoder>
#include <QtCore/QPointer>
#include "dataupdateengine-http.h"
#include <QtCore/QTimer>
#include <QHash>
#include <qmetatype.h>
#include "QueryTemplates.h"
#include "widgets/utils/GlobalAppSettings.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
#define QStringLiteral(A) QString::fromUtf8("" A "", sizeof(A) - 1)
using namespace QueryTemplates;


QPair<QString, bool> parseLogInResult(const QString& xml)
{
	QPair<QString, bool> result;
	QDomDocument doc;
	QString err;
	if (!doc.setContent(xml, &err)) {
		result.second = false;
		result.first = "login response error: " + err;
		return result;
	}
	QDomElement el = doc.elementsByTagName(QStringLiteral("login")).at(0).toElement();
	result.second = el.elementsByTagName(QStringLiteral("status")).at(0).toElement().text() == "200";
	if (result.second)
		result.first = el.elementsByTagName(QStringLiteral("session")).at(0).toElement().text();
	else
		result.first = el.elementsByTagName(QStringLiteral("message")).at(0).toElement().text();
	return result;
}





HttpUpdateEngine::HttpUpdateEngine(QString& Url, QObject* parent)
	: QObject(parent), url(Url), nextQueryId(0), delay(0), netManager(this)
{
	sendQuery("ping", Q_NULLPTR);
}

void HttpUpdateEngine::execQueryOutsideSession(QueryTemplates::QueryId id, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(0, id)) return;
	sendQuery(queryCache.value(id), awaiter);
}

void HttpUpdateEngine::execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(1, id)) return;
	sendQuery(queryCache.value(id).arg(arg1), awaiter);
}

void HttpUpdateEngine::execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, RequestAwaiter* awaiter)
{
	sendQuery(queryCache.value(id).arg(arg1).arg(arg2), awaiter);
}

void HttpUpdateEngine::execQueryOutsideSession(QueryTemplates::QueryId id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter)
{
	sendQuery(queryCache.value(id).arg(arg1).arg(arg2).arg(arg3), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(QueryTemplates::QueryId id, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(0, id)) return;
	sendQuery(queryCache.value(id).arg(sessionId), awaiter);
}


void HttpUpdateEngine::execQueryByTemplate(QueryTemplates::QueryId id, QString arg1, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(1, id)) return;
	sendQuery(queryCache.value(id).arg(sessionId).arg(arg1), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(QueryTemplates::QueryId id, QString arg1, QString arg2, RequestAwaiter* awaiter)
{

	if (!assertArgQuantity(2, id)) return;
	sendQuery(queryCache.value(id).arg(sessionId).arg(arg1).arg(arg2), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(QueryTemplates::QueryId id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(3, id)) return;
	sendQuery(queryCache.value(id).arg(sessionId).arg(arg1).arg(arg2).arg(arg3), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(QueryTemplates::QueryId id, int argc, QStringList argv, RequestAwaiter* awaiter)
{
	if (!assertArgQuantity(argc, id)) return;
	QString result = queryCache.value(id);
	result = result.arg(nextQueryId++).arg(sessionId);
	for (int i = 0; i < argc; ++i)
	{
		result = result.arg(argv.at(i));
	}
	sendQuery(
		result, awaiter
	);
}

void HttpUpdateEngine::execQueryByTemplate(const OverloadableQuery& oq, RequestAwaiter* awaiter)
{
	if (!oq.assertArgQuantity(0))return;
    QStringList t;
    sendQuery(oq.filterAndApply(t, sessionId), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(const OverloadableQuery& oq, QString arg1, RequestAwaiter* awaiter)
{
	if (!oq.assertArgQuantity(1))return;
    QStringList t;
    t << arg1;
    sendQuery(oq.filterAndApply(t, sessionId), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, RequestAwaiter* awaiter)
{
	if (!oq.assertArgQuantity(2))return;
    QStringList t;
    t << arg1 << arg2;
    sendQuery(oq.filterAndApply(t, sessionId), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(const OverloadableQuery& oq, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter)
{
	if (!oq.assertArgQuantity(3))return;
    QStringList t;
    t << arg1 << arg2 << arg3;
    sendQuery(oq.filterAndApply(t, sessionId), awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(const OverloadableQuery& oq, int argc, QStringList argv, RequestAwaiter* awaiter)
{
	if (!oq.assertArgQuantity(argc))return;
	sendQuery(oq.filterAndApply(argv, sessionId), awaiter);
}

void HttpUpdateEngine::setUrl(QString Url)
{
	url = Url;
	sendQuery(queryCache.value(ping), Q_NULLPTR);
}

QString HttpUpdateEngine::setSession(QString& requestResult)
{
	QPair<QString, bool> logRes = parseLogInResult(requestResult);
	if (logRes.second)
	{
		sessionId = logRes.first;
		return QString();
	}
	else
		return logRes.first;
}

bool HttpUpdateEngine::sessionReady()
{
	return !sessionId.isEmpty();
}

QString HttpUpdateEngine::getUrl()
{
	return url;
}

HttpUpdateEngine* HttpUpdateEngine::_instanse = Q_NULLPTR;
HttpUpdateEngine* HttpUpdateEngine::instanse()
{
#ifdef DEBUG
	if (_instanse == Q_NULLPTR)
	{
		QString defaultUrl = "http://92.115.237.226:60080/um/unitest.php";
		_instanse = new HttpUpdateEngine(defaultUrl, Q_NULLPTR);
	}
#else
	if (_instanse == Q_NULLPTR)
		_instanse = new HttpUpdateEngine(AppSettings->HttpUrl, Q_NULLPTR);
#endif
	return _instanse;
}

void HttpUpdateEngine::sendQuery(const QString& urlpath, RequestAwaiter* awaiter)
{
	if (awaiter != Q_NULLPTR)
	{
		if (awaiter->isAwaiting())
			return;
	}
	QString currUrl = url + "?delay=" + QString::number(delay) + "&cmd=" + urlpath;
	++delay;
#ifdef DEBUG
	detrace_NETREQSENT("sendQuery", currUrl, "");
#endif
	QNetworkRequest r;
	r.setRawHeader("Puller", "UamobiUtil");
	r.setUrl(QUrl::fromUserInput(currUrl));
	QNetworkReply* reply = netManager.get(r);
	if (!(awaiter == Q_NULLPTR))
		awaiter->setReplyToAwait(reply);
}



QString makeParamsFromList(QVector<QPair<QString, QString>>& vect)
{
	QString buffer;
	QTextStream stream(&buffer);
	QVector<QPair<QString, QString> >::iterator start = vect.begin();
	while (start != vect.end())
	{
		stream << makeParamsFromList(start->first, start->second);
		++start;
	}
	stream.flush();
	return buffer;
}

QString makeParamsFromList(QString& paramName, QString& paramVal)
{
	return  paramName + "-" + paramVal + "-";
}
const QString DATE_ENCODING_FORMAT(
	"dd.MM.yyyy"
);
