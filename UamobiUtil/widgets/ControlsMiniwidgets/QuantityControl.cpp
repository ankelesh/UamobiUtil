#include "QuantityControl.h"
#include "widgets/utils/ElementsStyles.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
QString QuantityControl::prepareAndReturnValue() const
{
    return QString::number(innerSpinbox->dvalue());
}

bool QuantityControl::parseAndSetValue(QString str)
{
	bool ok;
	double toSet = str.toDouble(&ok);
	if (ok)
	{
		innerSpinbox->setDValue(toSet);
		return true;
	}
	return false;
}

void QuantityControl::clear()
{
	innerSpinbox->setValue(0);
}

bool QuantityControl::valueAvailable() const
{
	return innerSpinbox->dvalue() > 0;
}

bool QuantityControl::hasFocus() const
{
    return innerSpinbox->hasFocus();
}

void QuantityControl::setListening(bool isListening)
{
    if (isListening)
    {

        innerSpinbox->setStyleSheet(LISTENING_CONTROL_STYLESHEET);
    }
    else
    {
        innerSpinbox->setStyleSheet("");
    }
}

QuantityControl::QuantityControl(bool isInt, QString& cname, QWidget* parent)
	:
	abs_control(cname, (isInt)? Int : Float), innerSpinbox(new BigButtonsSpinbox((isInt)? BigButtonsSpinbox::intspin : BigButtonsSpinbox::floatspin, parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(10000);
	innerSpinbox->setValue(0);
	innerSpinbox->setInfo(label);
	hide();
}

QuantityControl::QuantityControl(bool isInt, QString& assocBuffer, QString& cname, QWidget* parent)
	:abs_control(cname, assocBuffer, Int), innerSpinbox(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(10000);
	innerSpinbox->setValue(0);
	innerSpinbox->setInfo(label);
	hide();
}

QuantityControl::~QuantityControl()
{
	innerSpinbox->hide();
	innerSpinbox->blockSignals(true);
	innerSpinbox->deleteLater();
}

void QuantityControl::setFocus() const
{
	innerSpinbox->setFocus();
}

void QuantityControl::show()
{
	innerSpinbox->show();
}

void QuantityControl::hide()
{
	innerSpinbox->hide();
}
void QuantityControl::refresh()
{
#ifdef DEBUG
	detrace_METHCALL("QuantityControl::refresh |" << *associatedBuffer << "| of QC named " << name);
#endif
	bool ok;
	double toSet = associatedBuffer->toDouble(&ok);
	if (toSet - (int)toSet)
	{
		if (associatedBuffer->indexOf(QChar('.')) < associatedBuffer->count() - 4)
			associatedBuffer->chop(1);
	}
	if (ok)
	{
		detrace_SUCCESS;
		innerSpinbox->setDValue(toSet);
	}
	innerSpinbox->update();
}

void QuantityControl::installEventFilter(QObject* keyfilter)
{
	innerSpinbox->installEventFilter(keyfilter);
}

void QuantityControl::makeConnectionBetweenControls(abs_control* another)
{
#ifdef QT_VERSION5X
	QObject::connect(innerSpinbox, &BigButtonsSpinbox::returnPressed, another->myWidget(), QOverload<>::of(&QWidget::setFocus));
#else
	QObject::connect(innerSpinbox, SIGNAL(returnPressed()), another->myWidget(), SLOT(setFocus()));
#endif
}
