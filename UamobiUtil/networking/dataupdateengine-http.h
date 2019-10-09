#ifndef DATAUPDATEENGINEHTTP_H
#define DATAUPDATEENGINEHTTP_H

#include <QNetworkRequest>
#include <QNetworkReply>

#include "dataupdateengine.h"

/*
	Update:
		transformed some function signatures to NetCallArgs to use it via pointer

*/

class QVariant;

class HttpUpdateEngine : public DataUpdateEngine
{
	Q_OBJECT
private:
	QString url;
public:
	HttpUpdateEngine(QString& Url, QObject* parent);

	void setUrl(QString url);
	QString getUrl();
	bool sessionReady();
	Q_SLOT void initConnection();

	virtual void cancelPending();
	virtual void userUpdateList
	(
		QObject* receiver
		, const char* slot
		, const QString& placeCodeFilter
	);
	virtual void userLogIn
	(
		const QString& acc
		, const QString& passwd
		, QObject* receiver
		, const char* slot
	);
	virtual void userLogOut
	(
		QObject* receiver
		, const char* slot
	);

	virtual void modeList
	(
		QObject* receiver
		, const char* slot
		, const QString lang
	);
	virtual void modeSelect
	(
		const QString& mode
		, const QString& submode
		, QObject* receiver
		, const char* slot
	);
	virtual void placeList
	(
		QObject* receiver = Q_NULLPTR,
		const char* slot = Q_NULLPTR
	);
	virtual void placeSelect
	(
		const QString& place
		, QObject* receiver
		, const char* slot
	);

	virtual void docLock
	(
		const QString& subdocCode
		, QObject* receiver
		, const char* slot
	);
	virtual void docUnlock
	(
		bool save
		, QObject* receiver
		, const char* slot
	);

	virtual void docGetAllowedTypes
	(
		QObject* receiver
		, const char* slot
	);
	virtual void docSetFilter
	(
		const QString& filter
		, QObject* receiver
		, const char* slot
	);
	virtual void docGetAllowedToCreateTypes
	(
		QObject* receiver
		, const char* slot
	);
	virtual void docNewParent
	(
		const QString& sysfid
		, QObject* receiver
		, const char* slot
	);

	virtual void docSearchItems
	(
		const QString& text
		, const int page
		, QObject* receiver
		, const char* slot
	);
	virtual void itemGetInfo
	(
		const QString& code
		, QObject* receiver
		, const char* slot
	);
	/*virtual void itemGetLabel
	(
		const QString & code
	  , int qty
	  , QObject * receiver
	  , const char * slot
	);*/
	virtual void docGetResults
	(
		const QString& docId
		, QObject* receiver
		, const char* slot
	);
	virtual void docGetResults
	(
		int page
		, QObject* receiver
		, const char* slot
	);
	virtual void docResultGetBoxData
	(
		const QString& boxId
		, QObject* receiver
		, const char* slot
	);
	virtual void docResultEditByBoxID
	(
		const QString& boxId
		, const double qty
		, const double qtyOfPkg
		, const QString& lot
		, QObject* receiver
		, const char* slot
	);
	virtual void docResultDeleteItem
	(
		const QString& boxId
		, QObject* receiver
		, const char* slot
	);
	virtual void docUpdateParams
	(
		const QString& docId
		, const bool cancelled
		, const bool closed
		, const QString& comment
		, QObject* receiver
		, const char* slot
	);

	virtual void docListParentDocs
	(
		QObject* receiver
		, const char* slot
	);
	virtual void docListSubdocuments
	(
		QObject* receiver
		, const char* slot
	);

	virtual void invGetParentDocsList
	(
		QObject* receiver
		, const char* slot
	);
	virtual void getPhotoItemList
	(
		QObject* receiver
		, const char* slot
	);
	virtual void invNew
	(
		const QDate& date
		, const QString& parent
		, const QString& comment
		, QObject* receiver
		, const char* slot
	);
	virtual void invSubmit
	(
		const QString& code
		, const QString& qty
		, const QString& infofor
		, QObject* receiver
		, const char* slot
	);
	virtual void invSubmitExpDates
	(
		const QString& code
		, const QString& expDatesString
		, QObject* receiver
		, const char* slot
	);
	virtual void recListSuppliers(const QString& text, bool hasOrd, QObject*, const char*) override;
	virtual void recListOrders
	(
		const QString& supplier
		, QObject* receiver
		, const char* slot
	);
	virtual void recListOrdersByItem
	(
		const QString& barcode
		, QObject* receiver
		, const char* slot
	);
	virtual void recGetOrderInfo
	(
		const QString& ordercode
		, const QString& supplier
		, QObject* receiver
		, const char* slot
	);

	virtual void recNew
	(
		const QDate& date
		, const QString& parent
		, const QString& comment
		, QObject* receiver
		, const char* slot
	);
	virtual void recSubmit
	(
		const QString& code
		, const QString& num
		, QObject* receiver
		, const char* slot
	);
	virtual void recSubmitExpDates
	(
		const QString& code
		, const QString& expDatesString
		, QObject* receiver
		, const char* slot
	);
	virtual void recTestingInfo(
		QObject* receiver
		, const char* slot);
	virtual void docGetMultipleVersions(
		const QString& barcode,
		QObject* receiver,
		const char* slot
	);
	virtual void getWarehousesList(
		const QString& text,
		bool hasOrd,
		QObject* receiver = Q_NULLPTR,
		const char* slot = Q_NULLPTR
	) override;
	virtual void makeRequest(DataRequest* dr);
private:
	void sendQuery
	(
		const QString& urlpath
		, QObject* o
		, const char* slot
		, const QVariant& data = QVariant()
	);
	QString props2str(DataRequest* dr);
	//--
	enum NetReqUserDatas {
		DataSlot = QNetworkRequest::User + 2
		, DataInternal = QNetworkRequest::User + 3
	};

	unsigned m_nextQueryId;
	qint64 delay;
	QString m_sessionId;
	QNetworkAccessManager m_netMgr;
	QTime* delayTimer;

private slots:
	void requestFinish(QNetworkReply*);
	void onReplyError(QNetworkReply::NetworkError);
	void onRequestFinished();

signals:
	void answerReceived(QString);
};

#endif // DATAUPDATEENGINEHTTP_H
