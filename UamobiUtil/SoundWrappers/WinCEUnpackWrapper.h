#ifndef WINCEUNPACKWRAPPER_H
#define WINCEUNPACKWRAPPER_H
#ifdef Q_OS_WINCE
#include "AbsSoundWrapper.h"
#include "windows.h"
class WinCEUnpackWrapper : public AbsSoundWrapper
{
    Q_OBJECT
    // AbsSoundWrapper interface
protected:

    QVector<LPCWSTR> sources;
    QString lastSound;
    LPCWSTR _makePhysicalCopy(QString path);
    void _play(QString sourceName);
    int _prepare(QString souceName);
    void _play();
    void _clear();
    void _play(int which);
public:
    explicit WinCEUnpackWrapper(QStringList list, QObject* parent = Q_NULLPTR);
};




#endif
#endif // WINCEUNPACKWRAPPER_H
