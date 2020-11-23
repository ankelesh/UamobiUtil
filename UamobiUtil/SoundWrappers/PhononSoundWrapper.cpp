#include "PhononSoundWrapper.h"
#ifndef QT_VERSION5X
#ifndef Q_OS_WINCE
#include "widgets/utils/GlobalAppSettings.h"
typedef QVector<Phonon::MediaObject*> PMOVector;

void PhononSoundWrapper::_play(QString sourceName)
{
    if (AppSettings->notificationsVolume < 0.3)
        return;
    if (singleShotBuffer != Q_NULLPTR)
    {
        singleShotBuffer->deleteLater();
    }
    singleShotBuffer = Phonon::createPlayer(Phonon::Category::MusicCategory, Phonon::MediaSource(sourceName));
    singleShotBuffer->play();
}

int PhononSoundWrapper::_prepare(QString sourceName)
{
    soundGraphs.push_back(Phonon::createPlayer(Phonon::Category::MusicCategory, Phonon::MediaSource(sourceName)));
    //static_cast<Phonon::AudioOutput*>(soundGraphs.last())->setVolume(0.25);
    return soundGraphs.count() - 1;
}

void PhononSoundWrapper::_play()
{
    if (singleShotBuffer != Q_NULLPTR)
        singleShotBuffer->play();
}

void PhononSoundWrapper::_clear()
{
    for (PMOVector::iterator path = soundGraphs.begin(); path != soundGraphs.end(); ++path)
    {
        (*path)->deleteLater();
    }
    soundGraphs.clear();
}

void PhononSoundWrapper::_play(int which)
{
    if (which >= 0 && soundGraphs.count() && AppSettings->notificationsVolume > 0.3)
    {
        soundGraphs.at(which)->play();
    }
}

PhononSoundWrapper::PhononSoundWrapper(QStringList list, QObject *parent)
    : AbsSoundWrapper(list, parent), soundGraphs(), singleShotBuffer(Q_NULLPTR)
{
    soundGraphs.reserve(50);
    for (QStringList::iterator path = list.begin(); path != list.end(); ++path)
    {
        _prepare(*path);
    }
}
#endif
#endif
