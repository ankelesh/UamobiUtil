#include "AbsEntityPrototype.h"

int AbsRecEntity::extractEnumerable() const
{
	return 0;
}

bool AbsRecEntity::useAssociatedNetworkSendMethod(const QStringList& /*arguments*/, RequestAwaiter* /*awaiter*/) const
{
	return false;
}

bool AbsRecEntity::useAssociatedNetworkGetMethod(const QStringList& /*arguments*/, RequestAwaiter* /*awaiter*/) const
{
	return false;
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

QString AbsRecEntity::getId() const
{
	return extractId();
}

int AbsRecEntity::myType() const
{
	return class_id;
}

bool AbsRecEntity::sendAssociatedGetRequest(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	return useAssociatedNetworkGetMethod(arguments, awaiter);
}

bool AbsRecEntity::sendAssociatedPostRequest(const QStringList& arguments, RequestAwaiter* awaiter) const
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

template<>
bool NetRequestResponse<AbsRecEntity>::fromHeterogenicXmlObjects(const XmlObjects &objs, RecEntity prototype, int ares)
{
    alternative_result = ares;
    objects.clear();
    additionalObjects.clear();
    bool ok;
    for (int i = 0; i < objs.count(); ++i)
    {
        try
        {
            if (objs.at(i)->myOID() == prototype->myType())
            {
                ok = prototype->fromXmlObject(objs.at(i));
                if (!ok)
                {
                    isError = true;
                    errtext = "initialization non-throwing error";
                    return false;
                }
                // only difference between non-specialized and specialized versions
                objects.push_back(QSharedPointer<AbsRecEntity>(prototype->clone()));
            }
            else
            {
                additionalObjects.push_back(objs.at(i));
            }
        }
        catch (InitializationError & ie)
        {
            isError = true;
            errtext = ie.what();
            return false;
        }
    }
    return true;
}

template<>
bool NetRequestResponse<AbsRecEntity>::fromHomogenicXmlObjects(const XmlObjects &objs, RecEntity prototype, int ares)
{
    alternative_result = ares;
    bool ok = true;
    for (int i = 0; i < objs.count(); ++i)
    {
        try
        {
            ok = prototype->fromXmlObject(objs.at(i));
            if (!ok)
            {
                return false;
            }
            // only difference between non-specialized and specialized versions
            objects.push_back(RecEntity(prototype->clone()));
        }
        catch (InitializationError & ie)
        {
            isError = true;
            errtext = ie.what();
            return false;
        }
    }
    return true;
}

const char * InitializationError::what() const
#ifdef QT_VERSION5X
    noexcept
#endif
{
#ifdef Q_OS_WINCE
    return "init error of record";
#else
    return msg.c_str();
#endif
}
