#include "InputControlEntity.h"

InputControlEntity::InputControlEntity(QString nm, QString dv)
	: AbsRecEntity(UniformXmlObject::Control), name(nm), defaultValue(dv)
{
}

bool InputControlEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Control)
	{
		name = o.value("name");
		defaultValue = o.value("default");
		if (name.isEmpty())
			throw InitializationError("no control name provided", " ");
		return true;
	}
	return false;
}

QString InputControlEntity::makeTitle() const
{
	return name;
}

IdInt InputControlEntity::extractId() const
{
	return (IdInt)(this);
}

bool InputControlEntity::deepCompare(const AbsRecEntity* another) const
{
	if (myType() == another->myType())
	{
		return makeTitle() == another->getTitle();
	}
	return false;
}

AbsRecEntity* InputControlEntity::fabricate() const
{
	return new InputControlEntity(*this);
}

bool InputControlEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this > another;
}

bool InputControlEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this > another.data();
}
