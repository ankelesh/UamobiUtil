#pragma once
#include "AbsEntityPrototype.h"




class InputControlEntity : public AbsRecEntity
{
public:
	enum ControlTypes
	{
		None,
		Int,
		Date,
		Label,
		Float,
		String,
		Barcode
	};
	QString name;
	ControlTypes type;
	QString defaultValue;
	InputControlEntity(QString nm = QString(), QString Ctype = QString(), QString dv = QString());
	static ControlTypes intToType(const int cval);
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual int extractEnumerable() const override;
};
typedef QSharedPointer<InputControlEntity> InputControl;
typedef QVector<InputControl> IControlList;
