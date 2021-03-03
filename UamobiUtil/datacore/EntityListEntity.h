#pragma once
#include "AbsEntityPrototype.h"





class EntityListEntity : public AbsRecEntity
{
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual void overwriteId(QString new_id) override;
	virtual void overwriteTitle(QString new_title) override;
public:
	Records list;
	explicit EntityListEntity();
	explicit EntityListEntity(Records rlist);
	QString joinIds(QChar separator = ',') const;
};
typedef QSharedPointer<EntityListEntity> ERecordList;