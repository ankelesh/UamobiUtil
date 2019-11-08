#include "BigButtonsSpinbox.h"
#include "widgets/utils/ElementsStyles.h"

BigButtonsSpinbox::BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	buttonUp(new QPushButton(this)), buttonDown(new QPushButton(this)),
	infoLabel(new QLabel(this)),
	keyFilter(new filters::CaptureBackFilter(this)),
	coreSpinbox()
{
	switch (type)
	{
	case intspin:
		coreSpinbox = new QSpinBox(this);
		break;
	case timespin:
		coreSpinbox = new QTimeEdit(this);
	}
	sptype = type;
	this->setLayout(mainLayout);
	mainLayout->addWidget(buttonUp, 0,0,3,1);
	mainLayout->addWidget(infoLabel, 0, 1);
	mainLayout->addWidget(coreSpinbox, 1, 1, 2, 1);
	mainLayout->addWidget(buttonDown, 0, 2, 3, 1);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonUp->setIcon(QIcon(":/res/uparrow.png"));
	buttonDown->setIcon(QIcon(":/res/downarrow.png"));
	coreSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	buttonUp->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	buttonDown->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	infoLabel->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH / 3));
	infoLabel->setFont(makeFont(0.027));
	infoLabel->setAlignment(Qt::AlignCenter);
	coreSpinbox->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH * 0.6666));
	coreSpinbox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));

	coreSpinbox->installEventFilter(keyFilter);

	infoLabel->setStyleSheet("QLabel{border: 1px solid black;}");
	buttonUp->setStyleSheet(UP_SPINBOX_STYLESHEET);
	buttonDown->setStyleSheet(DOWN_SPINBOX_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(buttonUp, &QPushButton::pressed, coreSpinbox, &QSpinBox::stepUp);
	QObject::connect(buttonDown, &QPushButton::pressed, coreSpinbox, &QSpinBox::stepDown);
	QObject::connect(coreSpinbox, &QAbstractSpinBox::editingFinished, this, &BigButtonsSpinbox::editingDone);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &BigButtonsSpinbox::backRequire);
#else
	QObject::connect(buttonUp, SIGNAL(pressed()), coreSpinbox, SLOT(stepUp()));
	QObject::connect(buttonDown, SIGNAL(pressed()), coreSpinbox, SLOT(stepDown()));
	QObject::connect(coreSpinbox, SIGNAL(editingFinished()), this, SLOT(editingDone()));
	QObject::connect(keyFilter, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	QSpinBox* isp;
	QTimeEdit* tsp;
	switch (sptype)
		//RTTI used to connect right signals
	{
	case intspin:
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
	case timespin:
		tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			tsp->setDisplayFormat("HH:mm:ss");
#ifdef QT_VERSION5X
			QObject::connect(tsp, &QTimeEdit::timeChanged, this, &BigButtonsSpinbox::timeValueChanged);
#else
			QObject::connect(tsp, SIGNAL(timeChanged(QTime)), this, SLOT(timeValueChanged(QTime)));
#endif
		}
	default:
		break;
	}
}

void BigButtonsSpinbox::setMinimum(int min)
{
	if (sptype == intspin) {		// }
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMinimum(min);
		}
	}
}

void BigButtonsSpinbox::setMaximum(int max)
{
	if (sptype == intspin) {
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			isp->setMaximum(max);
		}
	}
}

void BigButtonsSpinbox::setValue(int val)
{
	if (sptype == intspin) {
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			isp->setValue(val);
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
	if (sptype == intspin) {
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			return isp->value();
		}
	}
	return 0;
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

bool BigButtonsSpinbox::hasFocus() const
{
	return coreSpinbox->hasFocus();
}

void BigButtonsSpinbox::setInfo(QString& str)
{
	infoLabel->setText(str);
}

void BigButtonsSpinbox::timeValueChanged(const QTime& t)
{
	emit timeChanged(t);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::editingDone()
{
	emit returnPressed();
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