#pragma once
#include <QObject>
#include <QStringList>
#include <qvector.h>



class AbsSoundWrapper : public QObject
{
    Q_OBJECT
protected:
    QStringList sourceFilenames;
    virtual void _play(QString sourceName) = 0;
    virtual int _prepare(QString souceName) = 0;
    virtual void _play() = 0;
    virtual void _clear() = 0;
    virtual void _play(int which) = 0;
public:
    explicit AbsSoundWrapper(QObject* parent = Q_NULLPTR);
    explicit AbsSoundWrapper(QStringList names, QObject* parent = Q_NULLPTR);
    int prepare(QString sourceName);
    void clear();
    QVector<int> prepare(QStringList sourceNames);
    void play(QString sourceName);
    
public slots:
    void play();
    void play(int which);


};

