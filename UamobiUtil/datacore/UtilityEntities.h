#pragma once
#include "AbsEntityPrototype.h"




class SeparatorEntity : public AbsRecEntity
{
protected:
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual void overwriteTitle(QString new_title) override;
	virtual void overwriteId(QString new_id) override;

public:
	SeparatorEntity();
};