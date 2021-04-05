#include "UtilityEntities.h"

bool SeparatorEntity::fromUniXml(const UniformXmlObject& o)
{
	return true;
}

QString SeparatorEntity::makeTitle() const
{
	return QString();
}

QString SeparatorEntity::extractId() const
{
	return QString();
}

bool SeparatorEntity::deepCompare(const AbsRecEntity* another) const
{
	return this == another;
}

AbsRecEntity* SeparatorEntity::fabricate() const
{
	return new SeparatorEntity();
}

bool SeparatorEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this < another;
}

bool SeparatorEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this < another;
}

void SeparatorEntity::overwriteTitle(QString new_title)
{

}

void SeparatorEntity::overwriteId(QString new_id)
{
}

SeparatorEntity::SeparatorEntity()
	: AbsRecEntity(UniformXmlObject::Separator)
{
}
