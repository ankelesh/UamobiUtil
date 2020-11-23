#include "AbsSoundWrapper.h"



AbsSoundWrapper::AbsSoundWrapper(QObject *parent) : QObject(parent), sourceFilenames()
{

}

AbsSoundWrapper::AbsSoundWrapper(QStringList names, QObject *parent) : QObject(parent), sourceFilenames(names)
{

}

int AbsSoundWrapper::prepare(QString sourceName)
{
    if (sourceFilenames.contains(sourceName))
        return sourceFilenames.indexOf(sourceName);
    return _prepare(sourceName);
}

void AbsSoundWrapper::play(QString sourceName)
{
    _play(sourceName);
}

void AbsSoundWrapper::play()
{
    _play();
}

void AbsSoundWrapper::play(int which)
{
    _play(which);
}

void AbsSoundWrapper::clear()
{
    _clear();
}

QVector<int> AbsSoundWrapper::prepare(QStringList sourceNames)
{
    QVector<int> tmp;
    tmp.reserve(sourceNames.count());
    for (QStringList::iterator i = sourceNames.begin(); i < sourceNames.end(); ++i )
        tmp.push_back(_prepare(*i));
    return tmp;
}
