#pragma once
#include "AbsEntityPrototype.h"




class FullItemEntity : public AbsRecEntity
{
public:
	QString title;
	QString code;
	QString cmid;
	QString box;
    double qty;
	bool highlight;
	int controlType;
	FullItemEntity(QString Title = QString(), QString Code = QString(),
		QString Cmid = QString(), QString Box = QString(),
		QString Qty = QString(), QString Highlight = QString(), QString controlType = QString());
	static void sendGetRequest(int pagenumber, RequestAwaiter* awaiter, QString doc = QString());
	void sendDeleteThisRequest(RequestAwaiter* awaiter);
	void sendDeleteThisRequest(const QueryTemplates::OverloadableQuery& oq, RequestAwaiter* awaiter);
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual double extractEnumerable() const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
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
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
};
typedef QSharedPointer<ShortItemEntity> ShortItem;
typedef QVector<ShortItem> ShortItemList;
