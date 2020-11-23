#pragma once
#include "AbsSoundWrapper.h"

#define AppSounds SoundEffectPlayer::instanse()


class SoundEffectPlayer : public QObject
{
	Q_OBJECT
public:
	enum DefaultEffects { ItemScannedDSE, ErrorDSE};
protected:
	AbsSoundWrapper* wrapper;

	static SoundEffectPlayer* _instanse;
	static QStringList _defaultSoundFiles;
	SoundEffectPlayer();

public:
	int allocateEffect(QString path);
	static SoundEffectPlayer* instanse();
public slots:
	void play(int);
	void play(QString);


};