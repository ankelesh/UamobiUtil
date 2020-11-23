#include "ModernQtSoundWrapper.h"
#ifdef QT_VERSION5X
#include "widgets/utils/GlobalAppSettings.h"

void ModernQtSoundWrapper::_play(QString sourceName)
{
	singleShotBuffer->setSource(sourceName);
	singleShotBuffer->play();
}

int ModernQtSoundWrapper::_prepare(QString sourceName)
{
	effects.push_back(new QSoundEffect(this));
	effects.last()->setSource(QUrl::fromLocalFile(sourceName));
	return effects.count() - 1;
}

void ModernQtSoundWrapper::_play()
{
    singleShotBuffer->setVolume(AppSettings->notificationsVolume);
	singleShotBuffer->play();
}

void ModernQtSoundWrapper::_clear()
{
	for (QVector<QSoundEffect*>::iterator se = effects.begin(); se != effects.end(); ++se)
	{
		(*se)->deleteLater();
	}
	effects.clear();
	sourceFilenames.clear();
}

void ModernQtSoundWrapper::_play(int which)
{
	if (which >= 0 && which < effects.count())
	{
		int st = effects.at(which)->status();
        effects[which]->setVolume(AppSettings->notificationsVolume);
		effects.at(which)->play();
	}
}


ModernQtSoundWrapper::ModernQtSoundWrapper(QStringList fnames, QObject* parent)
	: AbsSoundWrapper(fnames, parent), effects(), singleShotBuffer(new QSoundEffect(this))
{
	effects.reserve(50);
	for (QStringList::iterator fn = sourceFilenames.begin(); fn < sourceFilenames.end(); ++fn)
	{
		effects.push_back(new QSoundEffect(this));
		effects.last()->setSource(QUrl::fromLocalFile(*fn));
		effects.last()->setVolume(1);
	}
	singleShotBuffer->setVolume(0.25);
}
#endif
