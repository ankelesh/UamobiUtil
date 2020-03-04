#pragma once
#include "AbsEntityPrototype.h"
#include "networking/QueryTemplates.h"





class NamedIdEntity : public AbsRecEntity
	// proxy class for entities holding same data sets
{
public:
	QString id;
	QString name;
	
    NamedIdEntity(int subclass_id, QString i = QString(), QString nm = QString());
protected:
	//Inherited via AbsRecEntity
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
};

class PlaceEntity : public NamedIdEntity
{
public:
    PlaceEntity(QString id = QString(), QString nm = QString());

	void adjustQueryAndUseGet(QueryTemplates::QueryId quid, RequestAwaiter* awaiter);
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments,RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
	virtual AbsRecEntity* fabricate() const override;
};

typedef QSharedPointer<PlaceEntity> Place;
typedef QVector< QSharedPointer<PlaceEntity> > PlaceList;

class GroupEntity : public NamedIdEntity
{
public:
    GroupEntity(QString id = QString(), QString nm = QString());
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
	virtual AbsRecEntity* fabricate() const override;
};

typedef QSharedPointer<GroupEntity> Group;
typedef QVector< QSharedPointer<GroupEntity> > GroupList;

class StillageEntity : public NamedIdEntity
{
public:
    StillageEntity(QString id = QString(), QString nm = QString());
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
	virtual AbsRecEntity* fabricate() const override;
};
typedef QSharedPointer<StillageEntity> Stillage;
typedef QVector< QSharedPointer<StillageEntity> > StillageList;

class UserEntity : public AbsRecEntity
{
public:
	QString name;
	QString login;
	
	UserEntity(QString Name = QString(), QString Login = QString());
	void sendLoginRequest(const QString& password, RequestAwaiter* awaiter) const;
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
};

typedef QSharedPointer<UserEntity> User;
typedef QVector<QSharedPointer<UserEntity> > UserList;

class DocTypeEntity : public NamedIdEntity
{
public:
	bool isSelected;
    DocTypeEntity(QString Id = QString(), QString Name = QString(),bool selected = false);
	static void sendGetRequest(RequestAwaiter* awaiter);
	static void sendFilterList(QString ids, RequestAwaiter* awaiter);
protected:
	// Inherited via NamedIdEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
	virtual int extractEnumerable() const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
};

typedef QSharedPointer<DocTypeEntity> DocType;
typedef QVector<DocType> DocTypeList;

class BarcodeEntity : public NamedIdEntity
{
public:
	BarcodeEntity(QString code = QString(), QString name = QString());
protected:
	// Inherited via NamedIdEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
};

typedef QSharedPointer<BarcodeEntity> Barcode;

class InvoiceEntity : public NamedIdEntity
{
public:
	InvoiceEntity(QString code = QString(), QString name = QString());
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
};

typedef QSharedPointer<InvoiceEntity> Invoice;
