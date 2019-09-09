#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#ifdef GUI_APP
#include <QMessageBox>
#endif
#include <QTimer>
#include <QDataStream>
#include <QFile>
#include <QTextDecoder>
#include <QPointer>
#include <QDebug>

#include "global.h"
#include "xml_funcs.h"
#include "dataupdateengine-http.h"
#include "debugtrace.h"


struct SlotPointer
{
	SlotPointer(QObject *o, const QByteArray &slot)
	{
		this->obj  = o;
		this->slot = slot;
		this->id   = maxid++;
	}

	SlotPointer()
		:obj(0)
	{
		this->id   = maxid++;
	}

	static unsigned maxid;
	unsigned id;
	QPointer<QObject> obj;
	QByteArray slot;
};

unsigned SlotPointer::maxid = 0;

Q_DECLARE_METATYPE(SlotPointer)

HttpUpdateEngine::HttpUpdateEngine(QString & Url, QObject* parent )
    : DataUpdateEngine( parent ), url(Url), m_nextQueryId( 0 ), delay( 0 )
{
    //	connect(&m_netMgr, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinish(QNetworkReply*)));
    delayTimer = new QTime();
    //--
	
    detrace_DCONSTR("HttpUpdateEngine")
    QTimer::singleShot( 0, this, SLOT( initConnection() ) );
}

void HttpUpdateEngine::setUrl(QString Url)
{
	url = Url;
	initConnection();
}
bool HttpUpdateEngine::sessionReady()
{
	return !m_sessionId.isEmpty();
}
QString HttpUpdateEngine::getUrl()
{
	return url;
}

void HttpUpdateEngine::cancelPending()
{
    detrace_METHCALL("HttpUpdateEngine::cancelPending")
    m_nextQueryId = SlotPointer::maxid + 1;
}

void HttpUpdateEngine::initConnection()
{
    detrace_METHCALL("HttpUpdateEngine::initConnection")
    sendQuery( "ping", 0, 0 );
}

void HttpUpdateEngine::userUpdateList(
    QObject* receiver
  , const char* slot
  , const QString& placeCodeFilter
) {
    detrace_METHCALL("HttpUpdateEngine::userUpdateList")
    sendQuery(
        "list_users" + ( placeCodeFilter.isEmpty() ? QString() : "&place=" + placeCodeFilter )
      , receiver
      , slot
    );
}

void HttpUpdateEngine::userLogIn(
    const QString& acc
  , const QString& passwd
  , QObject* receiver
  , const char* slot
) {
    detrace_METHCALL("HttpUpdateEngine::userLogin")
    sendQuery(
        "log_in&username=" + acc + "&password=" + passwd
      , receiver
      , slot
      , QString( "LogIn" )
    );
}

void HttpUpdateEngine::userLogOut( QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::userLogOut")
    sendQuery(
        "log_out&session=" + m_sessionId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::modeList(
    QObject *receiver
  , const char *slot
  , const QString lang
) {
    detrace_METHCALL("HttpUpdateEngine::modeList")
    int iIndex = lang.indexOf( '.' );
    //--
    sendQuery(
        "list_modes&session=" + m_sessionId + "&lang=" + lang.left( iIndex ).toLower()
      , receiver
      , slot
    );
}

void HttpUpdateEngine::modeSelect(
    const QString& mode
  , const QString& submode
  , QObject* receiver
  , const char* slot
) {
    detrace_METHCALL("HttpUpdateEngine::modeSelect")
    sendQuery(
        "select_mode&session=" + m_sessionId + "&mode=" + mode + "&submode=" + submode
      , receiver
      , slot
    );
}

void HttpUpdateEngine::placeList( QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::placeList")
    sendQuery(
        "list_places&session=" + m_sessionId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::placeSelect( const QString& place, QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::placeSelect")
    sendQuery(
        "select_place&session=" + m_sessionId + "&place=" + place
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docLock( const QString& subdocCode, QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::docLock")
    sendQuery(
        "doc_edit&session=" + m_sessionId + "&document=" + subdocCode
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docUnlock( bool save, QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::docUnlock")
    sendQuery(
        "doc_exit&session=" + m_sessionId + "&save=" + ( save ? "true" : "false" )
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docGetAllowedTypes( QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::docGetAllowedTypes")
    sendQuery(
        "doc_get_allowed_types&session=" + m_sessionId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docSetFilter( const QString& filter, QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::docSetFilter")
    sendQuery(
        "doc_set_filter&session=" + m_sessionId + "&filter=" + filter
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docGetAllowedToCreateTypes( QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::docGetAllowedToCreateTypes")
    sendQuery(
        "doc_get_allowed_to_create_types&session=" + m_sessionId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docNewParent(const QString &sysfid, QObject *receiver, const char *slot)
{
    detrace_METHCALL("HttpUpdateEngine::docNewParent")
	sendQuery("doc_parent_new"
				 "&session="+m_sessionId
				 +"&sysfid="+sysfid,
				 receiver, slot);
}

void HttpUpdateEngine::docSearchItems(
    const QString& text
  , const int page
  , QObject* receiver
  , const char* slot
) {
    detrace_METHCALL("HttpUpdateEngine::docSearchItems")
    sendQuery(
        "doc_search_items&session=" + m_sessionId
      + "&text=" + text.toLocal8Bit().toPercentEncoding()
      + "&page=" + QString::number( page )
      , receiver
      , slot
    );
}

void HttpUpdateEngine::itemGetInfo( const QString& code, QObject* receiver, const char* slot )
{
    detrace_METHCALL("HttpUpdateEngine::itemGetInfo")
    QString printerType = getSettingsObject()->value( "printer/type", "zebra" ).toString();
    //--
    sendQuery(
        "doc_get_item_info&session=" + m_sessionId + "&barcode=" + code + "&printer=" + printerType
      , receiver
      , slot
    );
}

/*void HttpUpdateEngine::itemGetLabel(
    const QString& code
  , int qty
  , QObject* receiver
  , const char* slot
) {
    QString printerType = getSettingsObject()->value( "printer/type", "zebra" ).toString();
    //--
    sendQuery(
        "doc_get_item_label&session=" + m_sessionId
      + "&barcode=" + code
      + "&qty=" + QString::number( qty )
      + "&printer=" + printerType
      , receiver
      , slot
    );
}*/

void HttpUpdateEngine::docGetResults(
    const QString & docId
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::docGetResult")
    sendQuery(
        "doc_list_items&session=" + m_sessionId + "&document=" + docId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docGetResults( int page, QObject * receiver, const char * slot )
{
    detrace_METHCALL("HttpUpdateEngine::docGetResult")
    sendQuery(
        "doc_list_items&session=" + m_sessionId
      + "&page=" + QString::number( page )
      + "&document="
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docResultGetBoxData
(
    const QString & boxId
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("docResultGetBoxData")
    sendQuery(
        "doc_result_get_box_data&session=" + m_sessionId + "&box_id=" + boxId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docResultEditByBoxID
(
    const QString & boxId
  , const double qty
  , const double qtyOfPkg
  , const QString & lot
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::docResultEditByBoxId")
    sendQuery(
        "doc_result_edit_by_box_id&session=" + m_sessionId
      + "&box_id=" + boxId + "&qty=" + QString::number( qty )
      + "&qty_of_pkg=" + QString::number( qtyOfPkg ) + "&lot=" + lot
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docResultDeleteItem
(
    const QString & boxId
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::DocResultDeleteItem")
    sendQuery(
        "doc_result_delete_item&session=" + m_sessionId + "&box_id=" + boxId
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docUpdateParams
(
    const QString & docId
  , const bool cancelled
  , const bool closed
  , const QString & comment
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::docUpdateParams")
    sendQuery(
        "doc_update_params&session=" + m_sessionId
      + "&document=" + docId
      + "&cancelled=" + bool2str( cancelled )
      + "&closed=" + bool2str( closed )
      + "&comment=" + comment
      , receiver
      , slot
    );
}

void HttpUpdateEngine::docListParentDocs( QObject * receiver, const char * slot)
{
    detrace_METHCALL("HttpUpdateEngine::docListParentDocs")
}

void HttpUpdateEngine::docListSubdocuments( QObject * receiver, const char * slot )
{
    detrace_METHCALL("HttpUpdateEngine::docListSubDocuments")
    sendQuery( "doc_list_subdocuments&session=" + m_sessionId, receiver, slot );
}

void HttpUpdateEngine::invGetParentDocsList( QObject * receiver, const char * slot )
{
    detrace_METHCALL("HttpUpdateEngine::invGetParentDocsList")
    sendQuery( "inv_list_parent_docs&session=" + m_sessionId, receiver, slot );
}

void HttpUpdateEngine::getPhotoItemList( QObject * receiver, const char * slot )
{
    detrace_METHCALL("HttpupdateEngint::getPhotoItemList")
    sendQuery( "get_list_photo_items&session=" + m_sessionId, receiver, slot );
}

void HttpUpdateEngine::invNew
(
    const QDate & date
  , const QString & parent
  , const QString & comment
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::invNew")
    sendQuery(
        "inv_new_doc&session=" + m_sessionId
      + "&date=" + date.toString( "dd.MM.yyyy" )
      + "&parent=" + parent
      + "&comment=" + comment
      , receiver
      , slot
    );
}

void HttpUpdateEngine::invSubmit
(
    const QString & code
  , const QString & qty
  , const QString & infofor
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::invNew")
    sendQuery(
        "inv_add_item&session=" + m_sessionId
      + "&code=" + code
      + "&qty=" + qty
      + "&show_code=" + infofor
      , receiver
      , slot
    );
}

void HttpUpdateEngine::invSubmitExpDates
(
    const QString & code
  , const QString & expDatesString
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::invSubmitExpDates")
    sendQuery(
        "inv_add_item_w_ed&session=" + m_sessionId
      + "&code=" + code
      + "&expdates=" + expDatesString
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recListSuppliers
(
    const QString & text
  , bool hasOrders
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recListSuppliers")
    sendQuery(
        "rec_list_suppliers&session=" + m_sessionId
      + "&text=" + text
      + "&hasorders=" + bool2str( hasOrders )
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recListOrders
(
    const QString & supplier
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recListOrders")
    sendQuery(
        "rec_list_orders&session=" + m_sessionId
      + "&supplier=" + supplier
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recListOrdersByItem
(
    const QString & barcode
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recListOrdersByItem")
    sendQuery(
        "rec_list_orders&session=" + m_sessionId
      + "&barcode=" + barcode
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recGetOrderInfo
(
    const QString & ordercode
  , const QString & supplier
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recGetOrderInfo")
    sendQuery(
        "rec_get_order_info&session=" + m_sessionId
      + "&order=" + ordercode
      + "&supplier=" + supplier
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recNew
(
    const QDate & date
  , const QString & parent
  , const QString & comment
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recNew")
    sendQuery(
        "rec_new_doc&session=" + m_sessionId
      + "&date=" + date.toString( "dd.MM.yyyy" )
      + "&parent=" + parent
      + "&comment=" + comment.toLocal8Bit().toPercentEncoding()
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recSubmit
(
    const QString & code
  , const QString & qty
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recSubmit")
    sendQuery(
        "rec_add_item&session=" + m_sessionId
      + "&barcode=" + code
      + "&qty=" + qty
      , receiver
      , slot
    );
}

void HttpUpdateEngine::recSubmitExpDates
(
    const QString & code
  , const QString & expDatesString
  , QObject * receiver
  , const char * slot
) {
    detrace_METHCALL("HttpUpdateEngine::recSubmitExpDates")
    sendQuery(
        "rec_add_item_w_ed&session=" + m_sessionId
      + "&code=" + code
      + "&expdates=" + expDatesString
      , receiver
      , slot
    );
}

void HttpUpdateEngine::sendQuery
(
    const QString & urlpath
  , QObject * o
  , const char * slot
  , const QVariant & data
) {
    detrace_METHCALL("HttpupdateEngine::sendQuery")
    //qDebug() << ">> HttpUpdateEngine::sendQuery start";
    delayTimer->start();
    detrace_METHEXPL("timer started, forming url")
    QString currurl = url
      + "?delay=" + QString( "%1" ).arg( delay ) + "&cmd=" + urlpath;
    detrace_METHDATAS("sendQuery", "url", <<currurl);

    detrace_OCREATED("QNetworkRequest", "HttpUpdateEngine::sendQuery")
    QNetworkRequest r;
    if( !data.isNull() ) {
        r.setAttribute( static_cast<QNetworkRequest::Attribute>( DataInternal ), data );
    }

    r.setRawHeader( "User-Agent", "UAMobi/1.999a2" );
    r.setUrl( QUrl::fromUserInput( currurl ) );
    SlotPointer sp(o, slot);
    m_nextQueryId = sp.id;
    r.setAttribute(static_cast<QNetworkRequest::Attribute>(DataSlot), qVariantFromValue(sp));
	QNetworkReply *reply = m_netMgr.get(r);

    connect( reply, SIGNAL( finished() ), SLOT( onRequestFinished() ) );
    connect( reply, SIGNAL( finished() ), reply, SLOT( deleteLater() ) );
    connect( reply, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onReplyError( QNetworkReply::NetworkError ) ) );
    connect( reply, SIGNAL( error( QNetworkReply::NetworkError ) ), reply, SLOT( deleteLater() ) );
    detrace_METHEXPL("Connected to network reply object")
}

void HttpUpdateEngine::requestFinish( QNetworkReply * reply )
{
    detrace_SLOTCALL("requestFinish", "HttpUpdateEngine")
    if(delayTimer){
        delay = delayTimer->elapsed();
    }

    reply->deleteLater();
    detrace_DCONSTR("QTextDecoder")
    QString res;
    QTextDecoder td(QTextCodec::codecForName("CP1251")); // %-)
    // считывает xml блок, полученный от БД;
    if ( !reply->error() )
        res = td.toUnicode( reply->readAll() );
    detrace_METHTEXTS("HttpUpdateEngine::requestFinish", "res", res)
    /*qDebug() << "REPLY'S RESULT: " << "\n"
        << reply->request().url().toString() << "\n" << res;*/

    SlotPointer sp = reply->request()
        .attribute( static_cast<QNetworkRequest::Attribute>( DataSlot ) )
        .value<SlotPointer>();

    if( !sp.slot.isNull() && !sp.obj.isNull() ) {
        QString errText;

        if( reply->error() == QNetworkReply::NoError ) {
            if( reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt() != 200 )
                errText =
                    tr( "Network error" )
                  + reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toString()
                  + " "
                  + reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        } else {
            errText = reply->errorString();
        }
        if( reply->request()
                .attribute( static_cast<QNetworkRequest::Attribute>( DataInternal ) )
                .toString() == "LogIn"
        ) {
            QString dummy;
            XmlFns::logIn(res, dummy, m_sessionId);
			//detrace_METHEXPL("session now: " << m_sessionId << " while res was: " << res)
        }
        //detrace_METHEXPL("something dark happened, invocation begins")
        //if( sp.)
        // убрана проверка для запуска parsDocExitErrors ( Мунтяну В. );
        //if (sp.id == m_nextQueryId) {
		detrace_METHINVOK("sp.obj", "sp.slot.data", "RequestFinish", "HttpUpdateEngine")
			detrace_METHDATAS("HttpUpdateEngine::requestFinish", "res, errtext ", << res << errText)
			emit answerReceived(res);
        QMetaObject::invokeMethod( sp.obj, sp.slot.data(), Q_ARG( QString, res ), Q_ARG( QString, errText ) );
        //}
    }
}

void HttpUpdateEngine::onReplyError(QNetworkReply::NetworkError)
{
    detrace_METHCALL("HttpUpdateEngine::onReplyError")
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		reply->disconnect(this);
		requestFinish(reply);
    }
}

void HttpUpdateEngine::onRequestFinished()
{
    detrace_METHCALL("HttpUpdateEngine::onRequestFinished")
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());	
    if (reply) {
		reply->disconnect(this);
		requestFinish(reply);
    }
}

void HttpUpdateEngine::makeRequest(DataRequest *dr)
{
    detrace_METHCALL("HttpUpdateEngine::makeRequest")
    //qDebug() << ">> HttpUpdateEngine::makeRequest start";
    //QString s = dr->property("cod").toString();
    detrace_METHEXPL("making url")
	QString url
			=getSettingsObject()->value("http_host", "127.0.0.1/um/or.php").toString()
            + "?cmd=" + "save_photo"
            + "&session=" + m_sessionId
			+ props2str(dr);
    detrace_METHDATAS("HttpUpdateEngine::makeRequest", "url", << url)
    //qDebug() << "QUERY" << url;
    detrace_OCREATED("QNetworkRequest", "HttpUpdateEngine::makeRequest")
	QNetworkRequest req;
	req.setUrl(QUrl::fromUserInput(url));
	req.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User), reinterpret_cast<quintptr>(dr));
	req.setRawHeader("User-Agent", "UAMobi/1.999a4");
	QNetworkReply *reply;
	if (!dr->property("big_data").isNull() && dr->property("big_data").canConvert<QByteArray>()) {
		req.setHeader(QNetworkRequest::ContentTypeHeader, "binary/octet-stream");
		reply = m_netMgr.post(req, dr->property("big_data").toByteArray());
	} else {
		reply = m_netMgr.get(req);
	}
    detrace_METHEXPL("reply connected to this")
	connect(reply, SIGNAL(finished()), SLOT(onRequestFinished()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onReplyError(QNetworkReply::NetworkError)));   
    //qDebug() << "<< HttpUpdateEngine::makeRequest end";
}
void HttpUpdateEngine::recTestingInfo(
        QObject * receiver
        , const char * slot)
{
    sendQuery(
        "get_testing_info&session=" + m_sessionId
      , receiver
      , slot
    );
}
void HttpUpdateEngine::docGetMultipleVersions(const QString &barcode, QObject *receiver, const char *slot)
{
    sendQuery("doc_get_mult_version&session=" + m_sessionId + "&barcode=" + barcode,
              receiver,
              slot);
}

QString HttpUpdateEngine::props2str(DataRequest *dr)
{
    detrace_METHCALL("HttpUpdateEngine::props2str")
    //qDebug() << ">> HttpUpdateEngine::props2str start";
    QString res;
    QList<QByteArray> props = dr->dynamicPropertyNames();
    for (int i = 0; i < props.length(); i++) {
        if (dr->property(props.at(i)).canConvert<QString>() && props.at(i) != "big_data")
            res += QString("&") + QString::fromUtf8(props.at(i).toLower()) + QString("=") + dr->property(props.at(i)).toString().toUtf8().toPercentEncoding();
    }
    //qDebug() << "<< HttpUpdateEngine::props2str end";
    return res;
}
