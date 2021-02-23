#include "InputControlEntity.h"

abs_control::ControlTypes _initCType(QString str)
{
	switch (str.count())
	{
	case 3:
		return abs_control::Int;
	case 4:
		return abs_control::Date;
	case 5:
		if (str.startsWith('f', Qt::CaseInsensitive))
			return abs_control::Float;
		else
			return abs_control::Label;
	case 6:
		return abs_control::String;
	case 7:
		return abs_control::Barcode;
	default:
		return abs_control::None;
	}
};


InputControlEntity::InputControlEntity(QString nm,QString ctype, QString dv)
	: AbsRecEntity(UniformXmlObject::Control), name(nm), type(), defaultValue(dv)
{
	type = _initCType(ctype);
}

InputControlEntity::InputControlEntity(QString nm, int Ctype, QString dv)
	: AbsRecEntity(UniformXmlObject::Control), name(nm), type(intToType(Ctype)), defaultValue(dv)
{
}

abs_control::ControlTypes InputControlEntity::intToType(const int cval)
{
	if (cval >= abs_control::None && cval <= abs_control::Barcode)
		return static_cast<abs_control::ControlTypes>(cval);
	return abs_control::None;
}
abs_control::ControlTypes InputControlEntity::guessType(const QString& s)
{
	return _initCType(s);
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
    return QString::number(reinterpret_cast<long long int>(this));
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

double InputControlEntity::extractEnumerable() const
{
	return type;
}
