#include "AbsSoundWrapper.h"



AbsSoundWrapper::AbsSoundWrapper(QObject *parent) : QObject(parent), sourceFilenames()
{

}

AbsSoundWrapper::AbsSoundWrapper(QStringList names, QObject *parent) : QObject(parent), sourceFilenames(names)
{

}

void AbsSoundWrapper::prepare(QString sourceName)
{
    _prepare(sourceName);
}

void AbsSoundWrapper::play(QString sourceName)
{
    _play(sourceName);
}

void AbsSoundWrapper::play()
{
    _play();
}

void AbsSoundWrapper::stop()
{
    _stop();
}

void AbsSoundWrapper::play(int which)
{
    _play(which);
}

void AbsSoundWrapper::clear()
{
    _clear();
}

void AbsSoundWrapper::prepare(QStringList sourceNames)
{
    for (QStringList::iterator i = sourceNames.begin(); i < sourceNames.end(); ++i )
        _prepare(*i);
}
