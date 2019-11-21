#include "ControlListWidget.h"
#include "widgets/utils/ElementsStyles.h"
ControlListWidget::ControlListWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	userInfo(new QLabel(this)), barcodeInfo(new QLabel(this)),
	addInfo(new QLabel(this)), controls(), footerLayout(new QHBoxLayout(this)),
	okButton(new MegaIconButton(this)), backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(userInfo);
	mainLayout->addWidget(barcodeInfo);
	mainLayout->addWidget(addInfo);
	mainLayout->addLayout(footerLayout);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);
	
	QFont scf = makeFont(0.04);
	userInfo->setFont(scf);
	userInfo->setAlignment(Qt::AlignCenter);
	userInfo->setText(tr("fill data please"));
	barcodeInfo->setFont(scf);
	barcodeInfo->setAlignment(Qt::AlignCenter);

	addInfo->setFont(scf);
	addInfo->setAlignment(Qt::AlignCenter);

	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setText(tr("submit"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

#ifdef QT_VERSION5X
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ControlListWidget::checkAndConfirmControls);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ControlListWidget::backRequired);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(checkAndConfirmControls()));
	QObject::connect(backButton, SIGNAL(clicked())), this, SLOT(backRequired()));
#endif
}

void ControlListWidget::clearControls()
{
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		delete (*(begin++));
	}
}

void ControlListWidget::useControls(QStringList& initstrings)
{
	QStringList::iterator begin = initstrings.begin();
	while (begin != initstrings.end())
	{
		abs_control* ctrl = fabricateControl(*(begin++), mainLayout, this);
		if (controls.count() > 0)
		{
			controls.last()->makeConnectionBetweenControls(ctrl);
		}
		controls.push_back(ctrl);
	}
}

void ControlListWidget::useControls(QStringList& initstrings, QStringList& cvalues)
{
	if (initstrings.count() != cvalues.count())
		return;
	QStringList::iterator begin = initstrings.begin();
	QStringList::iterator cvalbegin = cvalues.begin();
	while (begin != initstrings.end())
	{
		abs_control* ctrl = fabricateControl(*(begin++), mainLayout, this);
		if (controls.count() > 0)
		{
			controls.last()->makeConnectionBetweenControls(ctrl);
		}
		ctrl->setValue(*(cvalbegin++));
		controls.push_back(ctrl);
	}
}

QStringList ControlListWidget::getControlsValues()
{
	QStringList temp;
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		temp << (*(begin++))->getValue();
	}
	return temp;
}

QString ControlListWidget::getValueAt(int index)
{
	if (index >= 0 && index < controls.count())
		return controls.at(index)->getValue();
	return QString();
}

void ControlListWidget::emplaceControl(QString& initstr)
{
	controls << fabricateControl(initstr, mainLayout, this);
}

void ControlListWidget::setLabels(QString barcode, QString additional, QString uinfo)
{
	if (!uinfo.isEmpty())
		userInfo->setText(uinfo);
	addInfo->setText(additional);
	barcodeInfo->setText(barcode);
}

void ControlListWidget::clearLabels()
{
	addInfo->clear();
	barcodeInfo->clear();
}

ControlListWidget::~ControlListWidget()
{
	clearControls();
}

void ControlListWidget::checkAndConfirmControls()
{
	bool ok;
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		ok &= (*(begin++))->canGiveValue();
	}
	if (ok)
	{
		emit controlsConfirmed();
	}
}
