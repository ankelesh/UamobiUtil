#include "WinCEUnpackWrapper.h"
#ifdef Q_OS_WINCE
#include "widgets/utils/GlobalAppSettings.h"
#include <QDir>
#include <string>
#include <vector>
LPCWSTR copyFromQStr(QString & from)
{
    LPCWSTR unconverted = static_cast<LPCWSTR>(from.utf16());
    WCHAR* converted = new WCHAR[from.count()+1];
    wcscpy_s(converted, from.count()+1, unconverted);
    return converted;
}

QString _makePureFilename(QString& path)
{
    QStringList splitres = path.split("/");
    if (splitres.count() > 1)
        return splitres.last();
    return QString();
}

LPCWSTR WinCEUnpackWrapper::_makePhysicalCopy(QString path)
{
    QDir cdir = QDir::current();
    if (!cdir.cd("sounds"))
    {
        cdir.mkdir("sounds");
        cdir.cd("sounds");
    }
    QString pure_fname = _makePureFilename(path);
    if (!cdir.exists(pure_fname))
    {
        QFile::copy(path, cdir.absoluteFilePath(pure_fname));
    }
    return copyFromQStr(cdir.absoluteFilePath(pure_fname));
}

void WinCEUnpackWrapper::_play(QString sourceName)
{
    if (!sourceName.startsWith(':'))
    {
        if (AppSettings->notificationsVolume > 0.3)
            PlaySound(sourceName.utf16(), NULL, SND_FILENAME);
        lastSound = sourceName;
    }
}

int WinCEUnpackWrapper::_prepare(QString souceName)
{
    if (souceName.startsWith(':'))
        sources.push_back(_makePhysicalCopy(souceName));
    else
        sources.push_back(copyFromQStr(souceName));
    return sources.count() - 1;
}

void WinCEUnpackWrapper::_play()
{
    _play(lastSound);
}

void WinCEUnpackWrapper::_clear()
{

}

void WinCEUnpackWrapper::_play(int which)
{
    if (which >= 0 && sources.count() && AppSettings->notificationsVolume > 0.3)
    {
        PlaySound(sources.at(which), NULL, SND_FILENAME);
    }
}

WinCEUnpackWrapper::WinCEUnpackWrapper(QStringList list, QObject *parent)
    : AbsSoundWrapper(list, parent), sources()
{
    sources.reserve(50);
    for (QStringList::iterator path = list.begin(); path != list.end(); ++path)
    {
        _prepare(*path);
    }
}
#endif
