#include "ControlTranslator.h"

ControlTranslator::ControlTranslator()
	: QObject(Q_NULLPTR)
{
}

QString ControlTranslator::translate(QString& str)
{
	switch (str.count())
	{
	case 3:
		if (str.compare("qty"))
		{
			return tr("control_quantity");
		}
		break;
	case 6:
		if (str.compare("addqty"))
		{
			return tr("control_addqty");
		}
		break;
	default:
		break;
	}
	return str;
}



static ControlTranslator translator;

QString makeControlTranslation(QString& str)
{
	return translator.translate(str);
}
