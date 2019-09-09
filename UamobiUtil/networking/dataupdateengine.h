#ifndef UPDATEDATAENGINE_H
#define UPDATEDATAENGINE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTcpSocket>
#include <QStringList>

#include "photo/datarequest.h"
#include "global.h"

class QNetworkReply;

class DataUpdateEngine : public QObject
{
    Q_OBJECT
public:
    explicit DataUpdateEngine( QObject * parent = 0 );
    ~DataUpdateEngine() {}
    //--
    virtual void cancelPending() = 0;
    // meta
    virtual void userUpdateList
    (
        QObject * receiver
      , const char * slot
      , const QString & placeCodeFilter = QString()
    ) = 0;
    virtual void userLogIn
    (
        const QString & acc
      , const QString & passwd
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void userLogOut
    (
        QObject * receiver
      , const char * slot
    ) = 0;

    virtual void modeList
    (
        QObject * receiver
      , const char * slot
      , const QString lang
    ) = 0;
    virtual void modeSelect
    (
        const QString & mode
      , const QString & submode
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void placeList
    (
        QObject * receiver
      , const char * slot
    ) = 0;
    virtual void placeSelect
    (
        const QString & place
      , QObject * receiver
      , const char * slot
    ) = 0;
    // doc general
    virtual void docLock
    (
        const QString & subdocCode
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docUnlock
    (
        bool save
      , QObject * receiver
      , const char * slot
    ) = 0;

    virtual void docGetAllowedTypes
    (
        QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docSetFilter
    (
        const QString & filter
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docGetAllowedToCreateTypes
    (
        QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docNewParent
    (
        const QString & sysfid
      , QObject * receiver
      , const char * slot
    ) = 0;

    virtual void docSearchItems
    (
        const QString & text
      , const int page
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void itemGetInfo
    (
        const QString & code
      , QObject * receiver
      , const char * slot
    ) = 0;
    /*virtual void itemGetLabel
    (
        const QString & code
      , int qty
      , QObject * receiver
      , const char * slot
    ) = 0;*/
    virtual void docGetResults
    (
        const QString & docId
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docGetResults
    (
        int page
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docResultGetBoxData
    (
        const QString & boxId
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docResultEditByBoxID
    (
        const QString & boxId
      , const double qty
      , const double qtyOfPkg
      , const QString & lot
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docResultDeleteItem
    (
        const QString & boxId
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void docUpdateParams
    (
        const QString & docId
      , const bool cancelled
      , const bool closed
      , const QString & comment
      , QObject * receiver
      , const char * slot
    ) = 0;

    virtual void docListParentDocs
    (
        QObject * receiver
      , const char * slot
    ) = 0; // todo
    virtual void docListSubdocuments
    (
        QObject * receiver
      , const char * slot
    ) = 0;

    virtual void getPhotoItemList
    (
        QObject * receiver
      , const char * slot
    ) = 0;
    // inventory-specific
    virtual void invGetParentDocsList
    (
        QObject * receiver
      , const char * slot
    ) = 0;
    virtual void invNew
    (
        const QDate & date
      , const QString & parent
      , const QString & comment
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void invSubmit
    (
        const QString & code
      , const QString & qty
      , const QString & infofor
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void invSubmitExpDates
    (
        const QString & code
      , const QString & expDatesString
      , QObject * receiver
      , const char * slot
    ) = 0;
    // receipt-specific
    virtual void recListSuppliers
    (
        const QString & text
      , bool hasOrdersOnly
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recListOrders
    (
        const QString & supplier
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recListOrdersByItem
    (
        const QString & barcode
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recGetOrderInfo
    (
        const QString & ordercode
      , const QString & supplier
      , QObject * receiver
      , const char * slot
    ) = 0;

    virtual void recNew
    (
        const QDate & date
      , const QString & parent
      , const QString & comment
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recSubmit
    (
        const QString & code
      , const QString & num
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recSubmitExpDates
    (
        const QString & code
      , const QString & expDatesString
      , QObject * receiver
      , const char * slot
    ) = 0;
    virtual void recTestingInfo(
             QObject * receiver
            , const char * slot) = 0;
    virtual void docGetMultipleVersions(
            const QString & barcode,
            QObject * receiver,
            const char * slot
            ) =0;
    // photo-specific
    virtual void makeRequest( DataRequest * dr ) = 0;
    static DataUpdateEngine * getMain( QObject * parent = 0);
    static DataUpdateEngine * byName( QString name, QObject * parent = 0 );
};

#endif // UPDATEDATAENGINE_H
