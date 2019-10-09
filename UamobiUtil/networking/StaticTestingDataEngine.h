#pragma once

#include "networking/dataupdateengine.h"
#include <QtCore/QTimer>

class StaticTestingDataEngine : public DataUpdateEngine
{
private:
	QTimer* timer;
	QPair<QString, QString> pendingResponse;
	QPair<QObject*, const char*> receiverCoords;

	void pushQuery(const QString&, const QString&, QObject* receiver, const char* slot);
public:
	StaticTestingDataEngine(int delay);

	// Inherited via DataUpdateEngine
	virtual void cancelPending() override;

	virtual void userUpdateList(QObject* receiver, const char* slot, const QString& placeCodeFilter = QString()) override;

	virtual void userLogIn(const QString& acc, const QString& passwd, QObject* receiver, const char* slot) override;

	virtual void userLogOut(QObject* receiver, const char* slot) override;

	virtual void modeList(QObject* receiver, const char* slot, const QString lang) override;

	virtual void modeSelect(const QString& mode, const QString& submode, QObject* receiver, const char* slot) override;

	virtual void placeList(QObject* receiver, const char* slot) override;

	virtual void placeSelect(const QString& place, QObject* receiver, const char* slot) override;

	virtual void docLock(const QString& subdocCode, QObject* receiver, const char* slot) override;

	virtual void docUnlock(bool save, QObject* receiver, const char* slot) override;

	virtual void docGetAllowedTypes(QObject* receiver, const char* slot) override;

	virtual void docSetFilter(const QString& filter, QObject* receiver, const char* slot) override;

	virtual void docGetAllowedToCreateTypes(QObject* receiver, const char* slot) override;

	virtual void docNewParent(const QString& sysfid, QObject* receiver, const char* slot) override;

	virtual void docSearchItems(const QString& text, const int page, QObject* receiver, const char* slot) override;

	virtual void itemGetInfo(const QString& code, QObject* receiver, const char* slot) override;

	virtual void docGetResults(const QString& docId, QObject* receiver, const char* slot) override;

	virtual void docGetResults(int page, QObject* receiver, const char* slot) override;

	virtual void docResultGetBoxData(const QString& boxId, QObject* receiver, const char* slot) override;

	virtual void docResultEditByBoxID(const QString& boxId, const double qty, const double qtyOfPkg, const QString& lot, QObject* receiver, const char* slot) override;

	virtual void docResultDeleteItem(const QString& boxId, QObject* receiver, const char* slot) override;

	virtual void docUpdateParams(const QString& docId, const bool cancelled, const bool closed, const QString& comment, QObject* receiver, const char* slot) override;

	virtual void docListParentDocs(QObject* receiver, const char* slot) override;

	virtual void docListSubdocuments(QObject* receiver, const char* slot) override;

	virtual void getPhotoItemList(QObject* receiver, const char* slot) override;

	virtual void invGetParentDocsList(QObject* receiver, const char* slot) override;

	virtual void invNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot) override;

	virtual void invSubmit(const QString& code, const QString& qty, const QString& infofor, QObject* receiver, const char* slot) override;

	virtual void invSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot) override;

	virtual void recListSuppliers(const QString& text, const bool hasOrdersOnly, QObject* receiver, const char* slot) override;

	virtual void recListOrders(const QString& supplier, QObject* receiver, const char* slot) override;

	virtual void recListOrdersByItem(const QString& barcode, QObject* receiver, const char* slot) override;

	virtual void recGetOrderInfo(const QString& ordercode, const QString& supplier, QObject* receiver, const char* slot) override;

	virtual void recNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot) override;

	virtual void recSubmit(const QString& code, const QString& num, QObject* receiver, const char* slot) override;

	virtual void recSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot) override;

	virtual void recTestingInfo(QObject* receiver, const char* slot) override;

	virtual void docGetMultipleVersions(const QString& barcode, QObject* receiver, const char* slot) override;

	virtual void getWarehousesList(const QString& text, const bool hasOrd, QObject* receiver = Q_NULLPTR, const char* slot = Q_NULLPTR) override;

	virtual void makeRequest(DataRequest* dr) override;
public slots:
	void delayOut();
};