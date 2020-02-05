#pragma once
#include "AbsEntityPrototype.h"
#include <qregularexpression.h>



class FullItemEntity : public AbsRecEntity
{
public:
	QString title;
	QString code;
	QString cmid;
	QString box;
	int qty;
	bool highlight;

	FullItemEntity(QString Title = QString(), QString Code = QString(),
		QString Cmid = QString(), QString Box = QString(),
		QString Qty = QString(), QString Highlight = QString());
	static void sendGetRequest(int pagenumber, RequestAwaiter* awaiter, QString doc = QString());
	void sendDeleteThisRequest(RequestAwaiter* awaiter);
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual IdInt extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual int extractEnumerable() const override;
	virtual bool useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const override;
};
typedef QSharedPointer<FullItemEntity> FullItem;
typedef QVector<QSharedPointer< FullItemEntity>  > FullItemList;

class ShortItemEntity : public AbsRecEntity
{
public:
	QString code;
	QString name;

	ShortItemEntity(QString code = QString(), QString name = QString());
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual IdInt extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual bool useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const override;
};
typedef QSharedPointer<ShortItemEntity> ShortItem;
typedef QVector<ShortItem> ShortItemList;