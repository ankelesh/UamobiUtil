#include <QApplication>
#include <QKeyEvent>
#include <QDebug>


#include "emulatedkbdinputhelper.h"

EmulatedKbdInputHelper::EmulatedKbdInputHelper(QObject *parent) :
	AbstractInputHelper(parent), m_isInputEmulated(false)
{
}

bool EmulatedKbdInputHelper::run()
{
	qApp->installEventFilter(this);
	return true;
}

bool EmulatedKbdInputHelper::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease
		 || event->type() == QEvent::KeyPress
		 || event->type() == QEvent::Shortcut
		 || event->type() == QEvent::ShortcutOverride) {

		if (event->type() == QEvent::KeyPress)
			return m_isInputEmulated;

		if (event->type() == QEvent::KeyRelease) {
			QKeyEvent *e = static_cast<QKeyEvent*>(event);

        if (
				 (e->key() == Qt::Key_Enter
				  || e->key() == Qt::Key_Return
				  || e->key() == Qt::Key_Dollar
				  || (
					  (e->key() == Qt::Key_4)
					  && (e->modifiers() & Qt::ShiftModifier)
					  )
				  )
				 && m_isInputEmulated) {
				if (m_code[0] < ' ') // some honeywells send DLE after prefix $
					m_code = m_code.mid(1);
                emit newCode(m_code.trimmed());
				m_isInputEmulated = false;
				return true;
            }

			if (m_isInputEmulated) {
				m_code += e->text();
				return true;
			}
            if (
				 (e->key() == Qt::Key_Dollar
				  || (
					  (e->key() == Qt::Key_4)
					  && (e->modifiers() & Qt::ShiftModifier)
					  )
				  ) && !m_isInputEmulated
				 ) {  
                //qApp->focusWidget()->clearFocus(); // F.M. 16.05.2014 - вылет из программы при серии сканированиий без активности в приложении
                m_isInputEmulated = true;
                m_code.clear();
                m_code.reserve(24);
				return true;
            }
		}

		if (event->type() == QEvent::Shortcut)  {
			if (m_isInputEmulated)
				return true;
		}
		if (event->type() == QEvent::ShortcutOverride) {
			if (m_isInputEmulated)
				return true;
        }
    }
    return false;
}
