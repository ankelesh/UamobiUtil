#pragma once
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>

/*
	This file contains everything related to handling events. Mostly KeyEvents
		NoKeyEvents:
			Despite it's name it does not eat key releases: it just intercepts them and
			emits corresponding signals. Main goal of this class is to keep outer reactions
			of widget while shutting down it's own. Example of usage: class ScaningWidget.
			There is barcodelist - a ListWidget, which must not react to number key presses,
			but all numbers must be counted. So this filter intercepts them and puts into signals.

		CaptureBackFilter:
			This filter is lighter version of NoKeyEvents. Only reacts to back key.

*/

namespace filters
{
	class NoKeyEvents : public QObject	//	intercepts key events to emit own signals
	{
		Q_OBJECT
	protected:
		bool eventFilter(QObject* object, QEvent* ev)
		{
			// Captures key press events
			if (ev->type() == QEvent::KeyRelease)
			{
				QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
				// Sorts: now only return, back and numbers must be intercepted
				if (keyptr->key() == Qt::Key_Return || keyptr->key() == 0)
				{
					emit returnObtained();
				}
				else if (keyptr->key() == Qt::Key_Back || keyptr->key() == Qt::Key_Escape)
				{
					emit backRequired();
				}
				else
				{
					bool ok;
					keyptr->text().toInt(&ok);
					if (ok)
					{
						emit numberObtained(keyptr->text());
					}
				}
				return true;
			}
			return QObject::eventFilter(object, ev);
		}
	public:
		NoKeyEvents(QObject* ob) : QObject(ob) {};
	signals:
		void numberObtained(QString);	//	holds number ready to be appended
		void returnObtained();			//	return was pressed, or an unknown button corresponding
		// to pda scan button
		void backRequired();	//	back or escape was pressed
	};

	class CaptureBackFilter : public QObject
	{
		Q_OBJECT
	protected:
		bool eventFilter(QObject* object, QEvent* ev)
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
	public:
		CaptureBackFilter(QObject* ob) : QObject(ob) {};
	signals:
		void backRequired();
	};
}
