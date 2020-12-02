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
        return QKeySequence(Qt::Key((ch.unicode())));
}


bool BarcodeObserver::eventFilter(QObject* object, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::KeyPress:
	{
		QKeyEvent* temp = static_cast<QKeyEvent*>(event);
		if (active)
			if (_testPressureForBarcode(temp, object))
				return true;
		if (_testGlobalHotkeyPress(temp, object))
			return true;
	}
	break;
	case (QEvent::KeyRelease):
	{
		if (active)
			if (_testReleaseForBarcode(event))
				return true;
		QKeyEvent* temp = static_cast<QKeyEvent*>(event);
		if (_testGlobalHotkeyRelease(temp, object))
			return true;
		if (numberCatchingActive)
			_testNumberReleased(temp);
	}
	break;
	default:
		break;
	}
	return QObject::eventFilter(object, event);
	
}

bool BarcodeObserver::_testPressureForBarcode(QKeyEvent * ev, QObject* target)
{
	if (prefixFound)
	{
		if (ev->text().count() > 0)
		{
			if (ev->key() == suffix[0])
			{
				prefixFound = false;
				emit barcodeCaught(buffer);
				emit suffixCaught();
				ev->accept();
				buffer.clear();
				return true;
			}
			else
			{
				buffer += ev->text();
				ev->accept();
				return true;
			}
		}
	}
	else
	{
		if (ev->text().count() > 0)
		{
			if (ev->text().at(0) == prefix[0])
			{
				prefixFound = true;
				ev->accept();
				emit prefixCaught();
				return true;
			}
		}
	}
	return false;
}

bool BarcodeObserver::_testReleaseForBarcode(QEvent* ev)
{
	if (prefixFound)
	{
		ev->accept();
		return true;
	}
	else
		return false;
}

bool BarcodeObserver::_testGlobalHotkeyPress(QKeyEvent* ev, QObject* target)
{
	switch (ev->key())
	{

	default:
		return false;
	}
}

bool BarcodeObserver::_testGlobalHotkeyRelease(QKeyEvent* ev, QObject* target)
{
	switch (ev->key())
	{
	case Qt::Key_Escape:
	case Qt::Key_Back:
		emit escapeCaught();
		ev->accept();
		return true;
	case Qt::Key_Left:
	case Qt::Key_Right:
	case Qt::Key_Up:
	case Qt::Key_Down:
		emit arrowCaught(ev->key());
		return true;
	case Qt::Key_Return:
		emit returnCaught();
		return false;
	default:
		return false;
	}
}

bool BarcodeObserver::_testNumberReleased(QKeyEvent* ev)
{
	switch (ev->key())
	{
	case Qt::Key_0:
		emit numberPressed(0);
		break;
	case Qt::Key_1:
		emit numberPressed(0);
		break;
	case Qt::Key_2:
		emit numberPressed(0);
		break;
	case Qt::Key_3:
		emit numberPressed(0);
		break;
	case Qt::Key_4:
		emit numberPressed(0);
		break;
	case Qt::Key_5:
		emit numberPressed(0);
		break;
	case Qt::Key_6:
		emit numberPressed(0);
		break;
	case Qt::Key_7:
		emit numberPressed(0);
		break;
	case Qt::Key_8:
		emit numberPressed(0);
		break;
	case Qt::Key_9:
		emit numberPressed(0);
		break;
	default:
		return false;
	}
	return false;
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
}

void BarcodeObserver::deactivate()
{
	active = false;
}
void BarcodeObserver::catchNumbers()
{
	++numberCatchingActive;
}
void BarcodeObserver::stopNumberCatching()
{
	--numberCatchingActive;
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

void BarcodeObserver::init()
{
	qApp->installEventFilter(instanse());
}
