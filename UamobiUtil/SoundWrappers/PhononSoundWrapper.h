#pragma once
#include "AbsSoundWrapper.h"
#ifndef QT_VERSION5X
#ifndef Q_OS_WINCE
#include <Phonon>

class PhononSoundWrapper : public AbsSoundWrapper
{
    Q_OBJECT
    // AbsSoundWrapper interface
protected:
    QVector<Phonon::MediaObject*> soundGraphs;
    Phonon::MediaObject* singleShotBuffer;

    void _play(QString sourceName);
    int _prepare(QString souceName);
    void _play();
    void _clear();
    void _play(int which);
public:
    explicit PhononSoundWrapper(QStringList list, QObject* parent = Q_NULLPTR);

};
#endif
#endif
