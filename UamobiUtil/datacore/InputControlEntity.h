#pragma once
#include "AbsEntityPrototype.h"




class InputControlEntity : public AbsRecEntity
{
public:
	QString name;
	QString defaultValue;
	InputControlEntity(QString nm = QString(), QString dv = QString());
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual IdInt extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
};
typedef QSharedPointer<InputControlEntity> InputControl;
typedef QVector<InputControl> IControlList;
