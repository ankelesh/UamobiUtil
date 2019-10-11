#include "inframedWidget.h"
#define DEBUG
#ifdef DEBUG
#include <debugtrace.h>
#endif
bool inframedWidget::isExpectingControl(int)
{
#ifdef DEBUG
	detrace_METHEXPL("in inframed::isExp");
#endif
	return false;

}
void inframedWidget::keyReleaseEvent(QKeyEvent* kev)
{
#ifdef DEBUG
	detrace_METHCALL("inframed::keyReleaseEvent with " << (int)kev->key());
#endif
	bool ok;
	int control = kev->text().toInt(&ok);
	if (ok) {
		processControl(control - 1);
	}
	else if (kev->key() == Qt::Key::Key_Back || kev->key() == Qt::Key::Key_Escape)
	{
		if (!back())
		{
			emit backRequired();
		}
	}
}

bool inframedWidget::processControl(int c)
{
	return isExpectingControl(c);
}

bool inframedWidget::back()
{
	return false;
}

bool inframedWidget::giveSettings()
{
	return false;
}

void inframedWidget::show()
{
	QWidget::show();
}
