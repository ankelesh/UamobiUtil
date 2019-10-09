#ifndef DATAREQUEST_H
#define DATAREQUEST_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QStringList>

class DataUpdateEngine;

class DataRequest : public QObject
{
	Q_OBJECT

public:
	enum Request {
		UsrList,
		UsrLogIn,
		UsrLogOut,
		BarcodeInfo,
		RawMaterialsSearch,
		HistoryTransportNumbers,
		PrepareTransportInfo,
		SaveTransportInfo,
		SavePhoto,
		TransportTicket
	};

	void setRequestParam(const char* name, const QVariant& value);
	QVariant requestParam(const char* name) const;

	explicit DataRequest(Request rt, QObject* parent = 0);

	Request type();
	bool hasErrors();
	QString errorMessage();
	void setErrorMessage(const QString& str);
	QVariant data();
	void setData(QVariant& data);

public slots:
	void call(DataUpdateEngine* e);
	void setDone();

signals:
	void done(DataRequest*);

private:
	Request m_requestType;
	QVariant m_resultData;
	QString m_errMsg;
	static int activeRequestsCount;

public:
	struct User;
	struct ActLogIn;
	struct BarcodeGenericInfo;
	struct MapList;
	struct DocumentInfo;
	struct BooleanAnswer;
	struct PrintingData;

	typedef QList<DataRequest::User> UserList;
};

struct DataRequest::User
{
	User() : needPasswd(false) {}
	QString login;
	QString name;
	bool needPasswd;
};

struct DataRequest::ActLogIn
{
	bool loggedIn;
	QStringList allowedModes;
	//	QString message;
};

struct DataRequest::BarcodeGenericInfo
{
	QString barcode;
	QString htmlRichDescription;
};

struct DataRequest::MapList
{
	QList< QHash<QString, QString> > items;
	MapList() : from(0), to(0), page(0), isFirst(true), isLast(true) {}
	int from, to;
	int page;
	bool isFirst, isLast;
};

struct DataRequest::DocumentInfo
{
	DocumentInfo() : docId(0) {}

	int docId;
	int sysfid;
	QHash<QString, QVariant> custom;
};

struct DataRequest::BooleanAnswer
{
	BooleanAnswer() : result(false) {}
	bool result;
};

struct DataRequest::PrintingData
{
	enum Type {
		Unknown,
		BixolonSequence,
		CPCL
	};

	PrintingData() : type(Unknown) {}

	QString text;
	Type type;
};

Q_DECLARE_METATYPE(DataRequest::User)
Q_DECLARE_METATYPE(DataRequest::UserList)
Q_DECLARE_METATYPE(DataRequest::ActLogIn)
Q_DECLARE_METATYPE(DataRequest::BarcodeGenericInfo)
Q_DECLARE_METATYPE(DataRequest::MapList)
Q_DECLARE_METATYPE(DataRequest::DocumentInfo)
Q_DECLARE_METATYPE(DataRequest::BooleanAnswer)
Q_DECLARE_METATYPE(DataRequest::PrintingData)

#endif // DATAREQUEST_H
