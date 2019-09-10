#ifndef EMULATEDKBDINPUTHELPER_H
#define EMULATEDKBDINPUTHELPER_H

#include "abstractinputhelper.h"

class EmulatedKbdInputHelper : public AbstractInputHelper
{
	Q_OBJECT

public:
	explicit EmulatedKbdInputHelper(QObject *parent = 0);
	virtual bool run();

protected:
	bool eventFilter(QObject *, QEvent *event);

private:
	QString m_code;
	bool m_isInputEmulated;
};

#endif // EMULATEDKBDINPUTHELPER_H
