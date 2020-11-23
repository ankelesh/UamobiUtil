#include "SoundEffectPlayer.h"
#ifdef QT_VERSION5X
#include "ModernQtSoundWrapper.h"
#else
#ifndef Q_OS_WINCE
#include "PhononSoundWrapper.h"
#else
#include "WinCEUnpackWrapper.h"
#endif
#endif


QStringList _initDSF()
{
	QStringList temp;
    temp << ":/res/sound/scan.wav";
    temp << ":/res/sound/fail.wav";
	return temp;
}


QStringList SoundEffectPlayer::_defaultSoundFiles(_initDSF());

SoundEffectPlayer::SoundEffectPlayer()
	: QObject(Q_NULLPTR),
#ifdef QT_VERSION5X
	wrapper(new ModernQtSoundWrapper(_defaultSoundFiles, this))
#else
#ifdef Q_OS_WINCE
    wrapper(new WinCEUnpackWrapper(_defaultSoundFiles, this))
#else
    wrapper(new PhononSoundWrapper(_defaultSoundFiles, this))
#endif
#endif
{

}

int SoundEffectPlayer::allocateEffect(QString path)
{
	return wrapper->prepare(path);
}

SoundEffectPlayer* SoundEffectPlayer::_instanse = Q_NULLPTR;
SoundEffectPlayer* SoundEffectPlayer::instanse()
{
	if (_instanse == Q_NULLPTR)
		_instanse = new SoundEffectPlayer();
	return _instanse;
}

void SoundEffectPlayer::play(int which)
{
	wrapper->play(which);
}

void SoundEffectPlayer::play(QString path)
{
	wrapper->play(path);
}
