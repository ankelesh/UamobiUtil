#include "DateTimeControl.h"
#include "networking/dataupdateengine-http.h"
#include "widgets/utils/ElementsStyles.h"
QString DateTimeControl::prepareAndReturnValue() const
{
	if (type == InputControlEntity::Date)
		return innerSpinbox->getDate().toString(DATE_ENCODING_FORMAT);
	else
		return innerSpinbox->getDateTime().toString(DATETIME_ENCODING_FORMAT);
}

bool DateTimeControl::parseAndSetValue(QString str)
{
	if (type == InputControlEntity::Date)
		innerSpinbox->setDate(QDate::fromString(str, DATE_ENCODING_FORMAT));
	else
		innerSpinbox->setDateTime(QDateTime::fromString(str, DATETIME_ENCODING_FORMAT));
	return true;
}

void DateTimeControl::clear()
{
	innerSpinbox->setDateTime(QDateTime::currentDateTime());
}

bool DateTimeControl::valueAvailable() const
{
	return true;
}

QWidget* DateTimeControl::getInnerWidget() const
{
	return innerSpinbox;
}

void DateTimeControl::setFocus() const
{
	innerSpinbox->setFocus();
	innerSpinbox->selectAll();
}

void DateTimeControl::show()
{
	innerSpinbox->show();
}

void DateTimeControl::hide()
{
	innerSpinbox->hide();
}

DateTimeControl::DateTimeControl()
	: abs_control(Q_NULLPTR), innerSpinbox(Q_NULLPTR)
{
}

DateTimeControl::DateTimeControl(bool /*needsTime*/, QString cname, QWidget * Parent)
    : abs_control(Parent, InputControlEntity::Date, cname),
	innerSpinbox(new BigButtonsSpinbox(BigButtonsSpinbox::datespin, parent))
{
	innerSpinbox->setInfo(label);
	innerSpinbox->setStyleSheet(FOCUSED_DATETIMEEDIT_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(innerSpinbox, &BigButtonsSpinbox::returnPressed, this, &DateTimeControl::subcontrolEditingFinished);
#else
	QObject::connect(innerSpinbox, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
	hide();
}

DateTimeControl::~DateTimeControl()
{
	innerSpinbox->hide();
	innerSpinbox->blockSignals(true);
	innerSpinbox->deleteLater();
}
