#include "BigButtonsSpinbox.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QDoubleSpinBox>
#else
#include <QtGui/QDoubleSpinBox>
#endif
#include <cmath>
#include <qcalendarwidget.h>

#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif

void BigButtonsSpinbox::showEvent(QShowEvent* ev)
{
	QWidget::showEvent(ev);
	if (sptype == datespin)
	{
		QDateEdit* tsp = qobject_cast<QDateEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			QString tmp = "QDateEdit{"
				"border-radius: 1px;"
				"padding: 1px 1px 1px 1px;"
				"border: 1px solid black;"
				"}"
				"QDateEdit::drop-down {"
				"subcontrol-origin: padding;"
				"subcontrol-position: center right;"
				"width: " + QString::number((int)(coreSpinbox->width() / 3)) + " px;"
				"}"
				;
			tsp->setStyleSheet(tmp);
		}
	}
}
BigButtonsSpinbox::BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	buttonUp(new QPushButton(this)), buttonDown(new QPushButton(this)),
	infoLabel(new QLabel(this)),
	keyFilter(new filters::CaptureBackFilter(this)),
	coreSpinbox(),
	lastEmit(QTime::currentTime())
{
	switch (type)
	{
	case intspin:
		coreSpinbox = new QSpinBox(this);
		break;
	case timespin:
		coreSpinbox = new QTimeEdit(this);
		break;
	case floatspin:
		coreSpinbox = new QDoubleSpinBox(this);
		break;
	case datespin:
		coreSpinbox = new QDateEdit(this);
		break;
	case datetimespin:
		coreSpinbox = new QDateTimeEdit(this);
	}
	sptype = type;
	this->setLayout(mainLayout);
	mainLayout->addWidget(buttonUp, 0, 0, 3, 1);
	mainLayout->addWidget(infoLabel, 0, 1);
	mainLayout->addWidget(coreSpinbox, 1, 1, 2, 1);
	mainLayout->addWidget(buttonDown, 0, 2, 3, 1);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);


	coreSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	coreSpinbox->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH * 0.6666));
	coreSpinbox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	coreSpinbox->installEventFilter(keyFilter);

	if (sptype == datespin)
	{
		buttonUp->hide();
		buttonDown->hide();
	}
	else
	{
		buttonUp->setIcon(QIcon(":/res/uparrow.png"));
		buttonDown->setIcon(QIcon(":/res/downarrow.png"));
		buttonUp->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
		buttonDown->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
		buttonDown->setFocusPolicy(Qt::NoFocus);
		buttonUp->setFocusPolicy(Qt::NoFocus);
		buttonUp->setStyleSheet(UP_SPINBOX_STYLESHEET);
		buttonDown->setStyleSheet(DOWN_SPINBOX_STYLESHEET);
	}
	infoLabel->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH / 3));
	infoLabel->setFont(*FontAdapter::general());
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setStyleSheet("QLabel{border: 1px solid black;}");

#ifdef QT_VERSION5X
	QObject::connect(buttonUp, &QPushButton::pressed, coreSpinbox, &QAbstractSpinBox::stepUp);
	QObject::connect(buttonDown, &QPushButton::pressed, coreSpinbox, &QAbstractSpinBox::stepDown);
	QObject::connect(coreSpinbox, &QAbstractSpinBox::editingFinished, this, &BigButtonsSpinbox::editingDone);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &BigButtonsSpinbox::backRequire);
#else
	QObject::connect(buttonUp, SIGNAL(pressed()), coreSpinbox, SLOT(stepUp()));
	QObject::connect(buttonDown, SIGNAL(pressed()), coreSpinbox, SLOT(stepDown()));
	QObject::connect(coreSpinbox, SIGNAL(editingFinished()), this, SLOT(editingDone()));
	QObject::connect(keyFilter, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif

	switch (sptype)
		//RTTI used to connect right signals
	{
	case intspin:
	{
		QSpinBox* isp;
		isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR) {
			isp->setSpecialValueText("");
#ifdef QT_VERSION5X
			QObject::connect(isp, QOverload<int>::of(&QSpinBox::valueChanged), this, &BigButtonsSpinbox::intValueChanged);
#else
			QObject::connect(isp, SIGNAL(valueChanged(int)), this, SLOT(intValueChanged(int)));
#endif
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp;
		dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setSpecialValueText("");
            dsp->setDecimals(3);
#ifdef QT_VERSION5X
			QObject::connect(dsp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BigButtonsSpinbox::doubleValueChanged);
#else
			QObject::connect(dsp, SIGNAL(valueChanged(double)), this, SLOT(doubleValueChanged(double)));
#endif
		}
		break;
	}
	case datespin:
	{
		QDateEdit* dsp;
		dsp = qobject_cast<QDateEdit*>(coreSpinbox);
		{
			if (dsp != Q_NULLPTR)
			{
				dsp->setSpecialValueText("");
				dsp->setDate(QDate::currentDate());
				dsp->setCalendarPopup(true);
				dsp->setDisplayFormat(QStringLiteral("dd:MM:yyyy")); 
#ifdef QT_VERSION5X
				QObject::connect(dsp, &QDateEdit::dateChanged, this, &BigButtonsSpinbox::dateChanged);
#else
                QObject::connect(dsp, SIGNAL(dateChanged(const QDate&)), this, SIGNAL(dateChanged(const QDate&)));
#endif
			}
		}
		break;
	}
	case timespin:
	{
		QTimeEdit* tsp;
		tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		{
			if (tsp != Q_NULLPTR)
			{
				tsp->setSpecialValueText("");
				tsp->setTime(QTime::currentTime());
				tsp->setDisplayFormat(QStringLiteral("HH:mm"));
#ifdef QT_VERSION5X
				QObject::connect(tsp, &QTimeEdit::timeChanged, this, &BigButtonsSpinbox::timeChanged);
#else
                QObject::connect(tsp, SIGNAL(timeChanged(const QTime&)), this, SIGNAL(timeChanged(const QTime&)));
#endif
			}
		}
		break;
	}
	case datetimespin:
		QDateTimeEdit* dtp;
		dtp = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		{
			if (dtp != Q_NULLPTR)
			{
				dtp->setSpecialValueText("");
				dtp->setDateTime(QDateTime::currentDateTime());
				dtp->setCalendarPopup(true);
				dtp->setDisplayFormat(QStringLiteral("dd:MM:yyyy"));
#ifdef QT_VERSION5X
				QObject::connect(dtp, &QDateTimeEdit::dateTimeChanged, this, &BigButtonsSpinbox::dateTimeChanged);
#else
                QObject::connect(dtp, SIGNAL(dateTimeChanged(const QDateTime&)), this, SIGNAL(dateTimeChanged(const QDateTime&)));
#endif
			}
		}
	default:
		break;
	}

}

void BigButtonsSpinbox::setMinimum(int min)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMinimum(min);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMinimum(min);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setMaximum(int max)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMaximum(max);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMaximum(max);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setValue(int val)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setValue(val);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setDValue(double val)
{
	switch (sptype) {
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*> (coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			int floored = (int)std::ceilf(val);
			isp->setValue(floored);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setTime(const QTime& tm)
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*> (coreSpinbox);
		if (tsp != 0)
		{
			tsp->setTime(tm);
		}
	}
}

int BigButtonsSpinbox::value() const
{
	switch (sptype)
	{
	case intspin:
	{	QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
	if (isp != Q_NULLPTR)
	{
		return isp->value();
	}
	break;
	}
	case floatspin:
	{	QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*> (coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
		return (int)dsp->value();
	}
	break;
	}
	default:
		break;
	}
	return 0;
}

double BigButtonsSpinbox::dvalue() const
{
	switch (sptype)
	{
	case floatspin:
	{	QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
		return dsp->value();
	}
	break; }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			return (double)isp->value();
		}
		break;
	}
	}
	return 0.0;
}

QTime BigButtonsSpinbox::time()
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			return tsp->time();
		}
	}
	return QTime();
}

void BigButtonsSpinbox::setDisplayFormat(const QString& tf)
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			tsp->setDisplayFormat(tf);
		}
	}
}

QDate BigButtonsSpinbox::getDate()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			return de->date();
	}
	return QDate();
}

void BigButtonsSpinbox::setDate(const QDate& dt)
{
	if (sptype == datespin || sptype == datetimespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setDate(dt);
	}
}

QDateTime BigButtonsSpinbox::getDateTime()
{
	if (sptype == datespin || sptype == datetimespin || sptype == timespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			return de->dateTime();
	}
	return QDateTime();
}

void BigButtonsSpinbox::setDateTime(const QDateTime& dt)
{
	if (sptype == datespin || sptype == datetimespin || sptype == timespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setDateTime(dt);
	}
}

bool BigButtonsSpinbox::hasFocus() const
{
	return coreSpinbox->hasFocus();
}

void BigButtonsSpinbox::setInfo(QString& str)
{
	infoLabel->setText(str);
}

void BigButtonsSpinbox::selectAll()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setSelectedSection(QDateTimeEdit::DaySection);
		return;
	}
	coreSpinbox->selectAll();
}

void BigButtonsSpinbox::setStyleSheet(const QString& st)
{
	coreSpinbox->setStyleSheet(st);
}


void BigButtonsSpinbox::timeValueChanged(const QTime& t)
{
	emit timeChanged(t);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::doubleValueChanged(double v)
{
	emit dvalueChanged(v);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::editingDone()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
		{
			if (de->calendarWidget()->hasFocus())
				return;
		}
		
	}
	if (lastEmit.msecsTo(QTime::currentTime()) > 50)
	{
		emit returnPressed();
		lastEmit = QTime::currentTime();
	}
}

void BigButtonsSpinbox::backRequire()
{
	emit backRequired();
}

void BigButtonsSpinbox::setFocus() const
{
	coreSpinbox->setFocus();
}

void BigButtonsSpinbox::intValueChanged(int t)
{
	emit ivalueChanged(t);
	emit valueChanged(coreSpinbox->text());
}
