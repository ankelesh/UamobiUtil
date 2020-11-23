#pragma once
#include "AbsSoundWrapper.h"
#ifdef QT_VERSION5X
#include <QSoundEffect>
#include <qvector.h>


class ModernQtSoundWrapper : public AbsSoundWrapper
{
    Q_OBJECT
protected:
    QVector<QSoundEffect*> effects;
    QSoundEffect* singleShotBuffer;
    // Inherited via AbsSoundWrapper
    virtual void _play(QString sourceName);
    virtual int _prepare(QString souceName);
    virtual void _play();
    virtual void _clear();
    virtual void _play(int which);
public:
    explicit ModernQtSoundWrapper(QStringList fnames = QStringList(),QObject* parent = Q_NULLPTR);



};

#endif
