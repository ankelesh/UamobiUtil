#include "EventsAndFilters.h"
#include <QApplication>
#ifdef Q_OS_ANDROID
#include <QInputMethod>
#endif
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif

bool filters::NoKeyEvents::eventFilter(QObject* object, QEvent* ev)
{
	// Captures key press events
	switch (ev->type())
	{
	case QEvent::KeyRelease:
	{
		QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		// Sorts: now only return, back and numbers must be intercepted
		switch (keyptr->key())
		{
		case Qt::Key_Return:
			emit returnObtained();
			break;
		case Qt::Key_Back:
		case Qt::Key_Escape:
			emit backRequired(); break;
		case Qt::Key_Backspace:
			emit eraseRequired();
			break;
		case Qt::Key_Dollar:
		case 0:
		case 33554431:
		case 16777248:
			emit unknownObtained(); break;
		default:
			if (keyptr->text() == "$")
				return true;
			emit numberObtained(keyptr->text());
			return true;
		}
	}
        Q_FALLTHROUGH();
	case QEvent::KeyPress:
	{
		QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		if (keyptr->key() == 0 || keyptr->key() == 33554431)
		{
			emit unknownObtained();
			return true;
		}
	}
        Q_FALLTHROUGH();
	default:
		return QObject::eventFilter(object, ev);
	}
}

bool filters::CaptureBackFilter::eventFilter(QObject* object, QEvent* ev)
{
	// Captures key press events
	if (ev->type() == QEvent::KeyRelease)
	{
		QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		// Sorts: now only return, back and numbers must be intercepted
		if (keyptr->key() == Qt::Key_Back || keyptr->key() == Qt::Key_Escape)
		{
			emit backRequired();
			return true;
		}
	}
	return QObject::eventFilter(object, ev);
}
bool filters::LineEditHelper::eventFilter(QObject* watched, QEvent* event)
{
	bool temp = QObject::eventFilter(watched, event);
#ifdef Q_OS_ANDROID
	if (event->type() == QEvent::KeyPress)
	{
		qApp->inputMethod()->commit();
	}
#endif
	return temp;
}

filters::GeneralPurposeFilter::CaptureFunction filters::GeneralPurposeFilter::makeCaptureFunction(captureTypes type)
{
	switch (type)
	{
	case (All):
        return &GeneralPurposeFilter::captAll;
	case (Symbol):
        return &GeneralPurposeFilter::captSymbol;
	case (Number):
        return &GeneralPurposeFilter::captNumber;
	case (Return):
        return &GeneralPurposeFilter::captReturn;
	case (Back):
        return &GeneralPurposeFilter::captBack;
	case (Scan):
        return &GeneralPurposeFilter::captScan;
	case (infPack):
        return &GeneralPurposeFilter::captInf;
#ifdef Q_OS_WIN
    default:
        return &GeneralPurposeFilter::captAll;
#endif
	}
}

bool filters::GeneralPurposeFilter::eventFilter(QObject* ob, QEvent* ev)
{

	if (ev->type() == QEvent::KeyRelease)
	{
		QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		return (*this.*captureFunction)(keyptr);
	}
	else
		return QObject::eventFilter(ob, ev);
}

bool filters::GeneralPurposeFilter::captReturn(QKeyEvent* keyptr)
{
	if (keyptr->key() == Qt::Key_Return)
	{
		emit returnPressed();
	}
	return true;
}

bool filters::GeneralPurposeFilter::captNumber(QKeyEvent* keyptr)
{
	bool ok;
	int temp = keyptr->text().toInt(&ok);
	if (ok)
	{
		emit numberPressed(temp);
	}
	return true;
}

bool filters::GeneralPurposeFilter::captSymbol(QKeyEvent* keyptr)
{
	if (keyptr->text().count() == 1)
	{
		emit symbolPressed(keyptr->text());
	}
	return true;
}

bool filters::GeneralPurposeFilter::captBack(QKeyEvent* keyptr)
{
	if (keyptr->key() == Qt::Key_Back || keyptr->key() == Qt::Key_Escape)
	{
		emit backPressed();
	}
	return true;
}

bool filters::GeneralPurposeFilter::captAll(QKeyEvent* keyptr)
{
	switch (keyptr->key())
	{
	case Qt::Key_Return:
		emit returnPressed();
		break;
	case Qt::Key_Back:
	case Qt::Key_Escape:
		emit backPressed();
		break;
	case Qt::Key_Dollar:
	case 0:
	case 33554431:
		emit scanPressed();
		break;
	default:
		bool ok;
		int temp = keyptr->text().toInt(&ok);
		if (ok)
		{
			emit numberPressed(temp);
		}
		emit symbolPressed(keyptr->text());
		break;
	}
	return true;
}

bool filters::GeneralPurposeFilter::captSome(QKeyEvent* keyPtr)
{
	for (int i = 0; i < typeLen; ++i)
	{
		(*this.*(someCaptureFunctions[i]))(keyPtr);
	}
	return true;
}

bool filters::GeneralPurposeFilter::captScan(QKeyEvent* keyptr)
{
	if (keyptr->key() == 0 || keyptr->key() == 33554431 || keyptr->key() == Qt::Key_Dollar)
	{
		emit scanPressed();
	}
	return true;
}

bool filters::GeneralPurposeFilter::captInf(QKeyEvent* keyptr)
{
	switch (keyptr->key())
	{
	case Qt::Key_Return:
		emit returnPressed();
		break;
	case Qt::Key_Back:
	case Qt::Key_Escape:
		emit backPressed();
		break;
	default:
		bool ok;
		int temp = keyptr->text().toInt(&ok);
		if (ok)
		{
			emit numberPressed(temp);
		}
		break;
	}
	return true;
}


filters::GeneralPurposeFilter::GeneralPurposeFilter(captureTypes type, QObject* parent)
	: QObject(parent)
{
	captureFunction = makeCaptureFunction(type);
}

filters::GeneralPurposeFilter::GeneralPurposeFilter(captureTypes type1, captureTypes type2, QObject* /*parent*/, captureTypes type3, captureTypes type4, captureTypes type5)
	: mytypes(), typeLen(0), captureFunction(&GeneralPurposeFilter::captAll),
	someCaptureFunctions()
{
	if (type1 == All) {
		captureFunction = &GeneralPurposeFilter::captAll;
	}
	else
	{
		captureFunction = &GeneralPurposeFilter::captSome;
		mytypes[0] = type1;
		int i = 0;
		if (type2 != All)
		{
			mytypes[i++] = type2;
		}
		if (type3 != All)
		{
			mytypes[i++] = type3;
		}
		if (type4 != All)
		{
			mytypes[i++] = type4;
		}
		if (type5 != All)
		{
			mytypes[i++] = type5;
		}
		typeLen = i;
		for (int j = 0; j < typeLen; ++j)
		{
			someCaptureFunctions[j] = makeCaptureFunction(mytypes[j]);
		}
	}
}
