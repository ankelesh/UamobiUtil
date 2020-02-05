#include "AbsEntityPrototype.h"

int AbsRecEntity::extractEnumerable() const
{
	return 0;
}

bool AbsRecEntity::useAssociatedNetworkSendMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	return useAssociatedNetworkSendMethod(arguments, awaiter);
}

bool AbsRecEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	return useAssociatedNetworkGetMethod(arguments, awaiter);
}

AbsRecEntity::AbsRecEntity(const int cl_id)
	: class_id(cl_id)
{
}

bool AbsRecEntity::fromXmlObject(const UniformXmlObject& o)
{
	return fromUniXml(o);
}

bool AbsRecEntity::fromXmlObject(const XmlObject o)
{
	return fromUniXml(*o);
}

QString AbsRecEntity::getTitle() const
{
	return makeTitle();
}

IdInt AbsRecEntity::getId() const
{
	return extractId();
}

int AbsRecEntity::myType() const
{
	return class_id;
}

bool AbsRecEntity::sendAssociatedGetRequest(QStringList& arguments, RequestAwaiter* awaiter) const
{
	return useAssociatedNetworkGetMethod(arguments, awaiter);
}

bool AbsRecEntity::sendAssociatedPostRequest(QStringList& arguments, RequestAwaiter* awaiter) const
{
	return useAssociatedNetworkSendMethod(arguments, awaiter);
}

int AbsRecEntity::getAttachedNumber() const
{
	return extractEnumerable();
}

bool AbsRecEntity::isSame(AbsRecEntity* another) const
{
	return deepCompare(another);
}

AbsRecEntity* AbsRecEntity::clone() const
{
	return fabricate();
}

bool AbsRecEntity::isHigher(AbsRecEntity* another) const
{
	return sortingCompare(another);
}

bool AbsRecEntity::isHigher(QSharedPointer<AbsRecEntity> another) const
{
	return sortingCompare(another);
}
