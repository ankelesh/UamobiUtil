#pragma once
#include "AbsEntityPrototype.h"

/*
	This file contains entities with unified data set - text with title and code
*/



class EntitledEntity : public AbsRecEntity
	// provides fields and getter with comparation to more specialized children
{
public:
	QString code;
	QString title;
	QString text;

	EntitledEntity(int subclassId, QString c = QString(), QString t = QString(), QString tx = QString());
protected:
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
};


class SupplierEntity : public EntitledEntity
	// represents supplier with order list
{
public:
	SupplierEntity(QString c = QString(), QString t = QString(), QString tx = QString());
protected:
	// Inherited via EntitledEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
};

typedef QSharedPointer<SupplierEntity> Supplier;
typedef QVector< QSharedPointer<SupplierEntity> > SupplierList;

class OrderEntity : public EntitledEntity
	// represents order with order description as text
{
public:
	OrderEntity(QString c = QString(), QString t = QString(), QString tx = QString());
protected:
	// Inherited via EntitledEntity
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
};
typedef QSharedPointer<OrderEntity> Order;
typedef QVector< QSharedPointer<OrderEntity> > OrderList;

class LesserDocumentEntity : public EntitledEntity
	// represents document without vast description
{
public:
	QString doctype; // represents type of this document

	LesserDocumentEntity(QString c = QString(), QString t = QString(), QString tx = QString(), QString doctype = QString());
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual AbsRecEntity* fabricate() const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;

};
