#pragma once
#include "AbsSoundWrapper.h"
#if lessThan(QT_VERSION,0x0300000)


class PhononSoundWrapper : public AbsSoundWrapper
{
    Q_OBJECT
    // AbsSoundWrapper interface
protected:
    void _play(QString sourceName);
    void _prepare(QString souceName);
    void _play();
    void _clear();
    void _play(int which);
    void _stop();
public:
    PhononSoundWrapper(QStringList list, QString s);
};





#endif


PhononSoundWrapper::PhononSoundWrapper(QStringList list, QString s)
{

}
