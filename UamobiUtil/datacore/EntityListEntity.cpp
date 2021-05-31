#include "EntityListEntity.h"
#include <QTextStream>
bool EntityListEntity::fromUniXml(const UniformXmlObject& o)
{
    return true;
}

QString EntityListEntity::makeTitle() const
{
    if (list.isEmpty())
        return QString("This is empty polylist of entities");
    else
        return list.first()->getTitle();
}

QString EntityListEntity::extractId() const
{
    if (list.isEmpty())
        return QString("This is empty polylist of entities");
    else
        return joinIds();
}

bool EntityListEntity::deepCompare(const AbsRecEntity* another) const
{
    return another->myType() == myType();
}

AbsRecEntity* EntityListEntity::fabricate() const
{
    return new EntityListEntity(list);
}

bool EntityListEntity::sortingCompare(const AbsRecEntity* another) const
{
    return this < another;
}

bool EntityListEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
    return this < another;
}

void EntityListEntity::overwriteId(QString new_id)
{
    if (!list.isEmpty())
        list.first()->setId(new_id);
}

void EntityListEntity::overwriteTitle(QString new_title)
{
    if (!list.isEmpty())
        list.first()->setTitle(new_title);
}

EntityListEntity::EntityListEntity()
    : AbsRecEntity(UniformXmlObject::EntityList), list()
{

}

EntityListEntity::EntityListEntity(Records rlist)
    : AbsRecEntity(UniformXmlObject::EntityList), list(rlist)
{
}

QString EntityListEntity::joinIds(QChar separator) const
{
    QString result;
    QTextStream rstream(&result);
    for (Records::const_iterator item = list.constBegin(); item != list.constEnd(); ++item)
    {
        rstream << (*item)->getId() << separator;
    }
    rstream.flush();
    result.chop(1);
    return result;
}


RecEntity EntityListEntity::takeFirst()
{
    RecEntity temp = list.first();
    list.remove(0);
    return temp;
}
