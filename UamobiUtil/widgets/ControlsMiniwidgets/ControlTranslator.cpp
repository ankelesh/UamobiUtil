#include "ControlTranslator.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif // DEBUG

ControlTranslator::ControlTranslator()
	: QObject(Q_NULLPTR)
{
}

QString ControlTranslator::translate(QString& str)
{
	detrace_METHCALL("translate control " << str << str.count() << " |");
	switch (str.count())
	{
	case 3:
		if (str == "qty")
		{
			detrace_METHEXPL("returning" << tr("control_quantity"));
			return tr("control_quantity");
		}
		break;
	case 6:
		if (str =="addqty")
		{
			return tr("control_addqty");
		}
		break;
	default:
		break;
	}
	return str;
}



ControlTranslator translator;

QString makeControlTranslation(QString& str)
{
	return translator.translate(str);
}
