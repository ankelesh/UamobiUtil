#include "InputControlEntity.h"

InputControlEntity::ControlTypes _initCType(QString str)
{
	switch (str.count())
	{
	case 3:
		return InputControlEntity::Int;
	case 4:
		return InputControlEntity::Date;
	case 5:
		if (str.startsWith('f', Qt::CaseInsensitive))
			return InputControlEntity::Float;
		else
			return InputControlEntity::Label;
	case 6:
		return InputControlEntity::String;
	case 7:
		return InputControlEntity::Barcode;
	default:
		return InputControlEntity::None;
	}
};


InputControlEntity::InputControlEntity(QString nm,QString ctype, QString dv)
	: AbsRecEntity(UniformXmlObject::Control), name(nm), type(), defaultValue(dv)
{
	type = _initCType(ctype);
}

InputControlEntity::ControlTypes InputControlEntity::intToType(const int cval)
{
	if (cval >= None && cval <= Barcode)
		return static_cast<ControlTypes>(cval);
	return None;
}

bool InputControlEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Control)
	{
		name = o.value("name");
		defaultValue = o.value("default");
		type = _initCType(o.value("type"));
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

QString InputControlEntity::extractId() const
{
	return QString::number((long long int)this);
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

int InputControlEntity::extractEnumerable() const
{
	return type;
}
