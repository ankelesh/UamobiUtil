#pragma once
#include <QObject>
#include <QStringList>




class AbsSoundWrapper : public QObject
{
    Q_OBJECT
protected:
    QStringList sourceFilenames;
    virtual void _play(QString sourceName) = 0;
    virtual void _prepare(QString souceName) = 0;
    virtual void _play() = 0;
    virtual void _clear() = 0;
    virtual void _play(int which) = 0;
    virtual void _stop() = 0;
public:
    explicit AbsSoundWrapper(QObject* parent = Q_NULLPTR);
    explicit AbsSoundWrapper(QStringList names, QObject* parent = Q_NULLPTR);
    void prepare(QString sourceName);
    void clear();
    void prepare(QStringList sourceNames);
    void play(QString sourceName);

public slots:
    void play();
    void stop();
    void play(int which);


};

