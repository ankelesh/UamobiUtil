#include "BarcodeObserver.h"
#include <qapplication.h>
#include "widgets/utils/GlobalAppSettings.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
QKeySequence _initiateSequence(QChar ch)
{
	if (ch.isLetterOrNumber())
		return QKeySequence(ch);
	if (ch == '\n')
	{
        return QKeySequence(Qt::Key_Return);
	}
	else
		return QKeySequence((Qt::Key)(ch.unicode()));
}


bool BarcodeObserver::eventFilter(QObject* object, QEvent* event)
{
	if (!active)
	{
		return QObject::eventFilter(object, event);
	}
	if (event->type() == QEvent::KeyPress)
	{
        QKeyEvent* temp = static_cast<QKeyEvent*>(event);
        #ifdef DEBUG
        detrace_METHEXPL("key event found");
        #endif
		if (prefixFound)
		{
			if (temp->text().count() > 0)
			{
				if (temp->key() == suffix[0])
				{
                    #ifdef DEBUG
                    detrace_METHEXPL("suffix found");
                    #endif
					prefixFound = false;
					emit barcodeCaught(buffer);
					emit suffixCaught();
                    #ifdef DEBUG
                    detrace_SIGNALINV("barcode caught " + buffer, "suffixCaught");
                    #endif
					event->accept();
					buffer.clear();
                    #ifdef DEBUG
                    detrace_METHEXPL("accepted and cleared");
                    #endif
					return true;
				}
				else
				{
                    #ifdef DEBUG
                    detrace_METHEXPL("attaching to buffer: " << temp->text());
                    #endif
					buffer += temp->text();
					event->accept();
					return true;
				}
			}
		}
		else
		{
            #ifdef DEBUG
            detrace_METHDATAS("keyEvent: " ,"text, key, count", << temp->text() << temp->key() << temp->count());
            #endif
            if (temp->text().count() > 0)
			{
                #ifdef DEBUG
                detrace_METHEXPL("passed count check");
                #endif
				if (temp->text().at(0) == prefix[0])
				{
                    #ifdef DEBUG
                    detrace_METHEXPL("prefix found");
                    #endif
					prefixFound = true;
					event->accept();
					emit prefixCaught();
					return true;
				}
				else
				{
					return QObject::eventFilter(object, event);
				}
			}
		}

	}
	else if (event->type() == QEvent::KeyRelease)
	{
		if (prefixFound)
		{
			event->accept();
			return true;
		}
	}
	return QObject::eventFilter(object, event);
}

BarcodeObserver::BarcodeObserver(QChar pref, QChar suff, QObject* parent)
	: QObject(parent), prefix(_initiateSequence(pref)), 
	suffix(_initiateSequence(suff)), buffer(), prefixFound(false), active(false)
{
}

void BarcodeObserver::resetCapture(QChar pref, QChar suff)
{
	prefix = _initiateSequence(pref);
	suffix = _initiateSequence(suff);
	buffer.clear();
	prefixFound = false;
}

void BarcodeObserver::activate()
{
	active = true;
#ifdef DEBUG
    detrace_METHEXPL("activated observer with p\\s pair " << prefix.toString() << " " << suffix.toString() << "|");
#endif
	qApp->installEventFilter(instanse());
}

void BarcodeObserver::deactivate()
{
#ifdef DEBUG
    detrace_METHCALL("BarcodeObserver::deactivate");
#endif
	active = false;
	qApp->removeEventFilter(instanse());
}
bool BarcodeObserver::activated()
{
	return active;
}
BarcodeObserver* BarcodeObserver::_instanse = Q_NULLPTR;
BarcodeObserver* BarcodeObserver::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new BarcodeObserver(QChar(AppSettings->scanPrefix), QChar(AppSettings->scanSuffix) , Q_NULLPTR);
	}
	return _instanse;
}
