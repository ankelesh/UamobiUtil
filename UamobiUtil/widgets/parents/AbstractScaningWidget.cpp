#include "AbstractScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "ScaningCore/BarcodeObserver.h"
void AbstractScaningWidget::useControls(QVector<QSharedPointer < InputControlEntity> >& cvals)
{
    switch (cvals.count())
	{
	case 0:
		switch (controlsAvailable)
		{
		case 2:
			innerLayout->removeWidget(second_control->myWidget());
			second_control.clear();
		case 1:
			innerLayout->removeWidget(first_control->myWidget());
			first_control.clear();
		case 0:
			setFocus();
			controlsRequired = false;
			submitButton->setDisabled(true);
			controlsAvailable = 0;
			controlsRequired = false;
		}
		return;
	case 1:

		if (controlsAvailable >= 1)
		{
			if (first_control->name == cvals.at(0)->name)
			{
				first_control->reset();
				first_control->show();
			}
			else
			{
				innerLayout->removeWidget(first_control->myWidget());
				first_control.clear();
                first_control = Control(fabricateControl(cvals.at(0)->name, innerLayout, innerWidget));
			}
			if (!second_control.isNull())
			{
				innerLayout->removeWidget(second_control->myWidget());
				second_control.clear();
				--controlsAvailable;
			}
		}
		else
		{
            first_control = Control(fabricateControl(cvals.at(0)->name, innerLayout, innerWidget));
			++controlsAvailable;
		}
		first_control->show();
		controlsRequired = true;
		submitButton->setDisabled(false);
		this->setFocus();
		controlsAvailable = 1;
		return;
	case 2:
	default:
		switch (controlsAvailable)
		{
		case 2:
			if (!(cvals.at(1)->name == second_control->name))
			{
				innerLayout->removeWidget(second_control->myWidget());
				second_control.clear();
			}
		case 1:
			if (!(cvals.at(1)->name == first_control->name))
			{
				innerLayout->removeWidget(first_control->myWidget());
				first_control.clear();
			}
		case 0:
			if (first_control.isNull())
			{
                first_control = Control(fabricateControl(cvals.at(0)->name, innerLayout, innerWidget));
			}
			else
			{
				first_control->reset();
			}
			if (second_control.isNull())
			{
                second_control = Control(fabricateControl(cvals.at(1)->name, innerLayout, innerWidget));
			}
			else
			{
				second_control->reset();
			}
		}
		controlsAvailable = 2;
	}
	controlsRequired = true;
	submitButton->setDisabled(false);
	this->setFocus();
	first_control->show();
	second_control->show();
}

void AbstractScaningWidget::wipe()
{
	barcodeField->clear();
}



AbstractScaningWidget::AbstractScaningWidget(int id, QWidget* parent)
	: IndependentBranchNode(id, parent), document(new FullDocumentEntity()),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), topPanelLayout(new QHBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)),
	barcodeField(new QLineEdit(innerWidget)),
	mainTextView(new QTextEdit(innerWidget)),
	buttonPanel(new QHBoxLayout(innerWidget)),
	quitButton(new MegaIconButton(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
	submitButton(new MegaIconButton(innerWidget)), searchButton(new MegaIconButton(innerWidget)),
	itemSuppliedValues(),
	first_control(), second_control(), controlsAvailable(0),
	awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(topPanelLayout);
	topPanelLayout->addWidget(quitButton);
	topPanelLayout->addWidget(userInfo);
	innerLayout->addWidget(barcodeField);
	innerLayout->addWidget(mainTextView);
	innerLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(searchButton);
	buttonPanel->addWidget(submitButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	topPanelLayout->setContentsMargins(0, 0, 0, 0);
	topPanelLayout->setSpacing(0);

	QFont scaledFont = makeFont(0.04);
	userInfo->setText(tr("scaning_widget_user_info"));
	userInfo->setAlignment(Qt::AlignCenter);
	userInfo->setFont(scaledFont);
	userInfo->setWordWrap(true);
    userInfo->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    userInfo->setMinimumWidth(calculateAdaptiveWidth(0.75));
    userInfo->setMaximumHeight(calculateAdaptiveButtonHeight(0.2));

	mainTextView->setText(tr("scaning_widget_filler_text"));
	mainTextView->setFont(scaledFont);
	mainTextView->setAcceptRichText(true);
	mainTextView->setReadOnly(true);
    mainTextView->setTextInteractionFlags(Qt::NoTextInteraction);
	mainTextView->setStyleSheet("QLabel{background-color: white; border: 1px solid black}");
	mainTextView->setAlignment(Qt::AlignTop);
	backButton->setText(tr("scaning_widget_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	submitButton->setText(tr("scaining_widget_submit"));
	submitButton->setIcon(QIcon(":/res/submit.png"));
	submitButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	searchButton->setText(tr("scaning_widget_search"));
	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);

	quitButton->setText(tr("quit"));
	quitButton->setIcon(QIcon(":/res/data.png"));
	quitButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
    quitButton->setMaximumWidth(calculateAdaptiveWidth(0.25));
    quitButton->setMinimumWidth(calculateAdaptiveWidth(0.25));
    quitButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.2));
    quitButton->setMinimumHeight(calculateAdaptiveButtonHeight(0.2));

	barcodeField->setFocus();

#ifdef QT_VERSION5X

	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backNeeded);
	QObject::connect(submitButton, &QPushButton::clicked, this, &AbstractScaningWidget::submitPressed);
	QObject::connect(barcodeField, &QLineEdit::returnPressed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(searchButton, &QPushButton::clicked, this, &AbstractScaningWidget::searchRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &AbstractScaningWidget::was_timeout);
	QObject::connect(quitButton, &MegaIconButton::clicked, this, &AbstractScaningWidget::quitNoSave);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(backNeeded()));
	QObject::connect(submitButton, SIGNAL(clicked()), this, SLOT(submitPressed()));
    QObject::connect(barcodeField, SIGNAL(returnPressed()), this, SLOT(barcodeConfirmed()));
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchRequired()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitNoSave()));
#endif
}

void AbstractScaningWidget::clear()
{
	mainTextView->clear();
	useControls(QVector< QSharedPointer< InputControlEntity> >());
}

void AbstractScaningWidget::setModeName(QString& name)
{
    modename = name;
}

void AbstractScaningWidget::was_timeout()
{
	userInfo->setText("scaning_timeout:" + QString::number(awaiter->getInterval()));
    hideProcessingOverlay();
}

void AbstractScaningWidget::quitNoSave()
{
#ifdef DEBUG
    detrace_METHCALL("quitNoSave");
#endif
	QMessageBox::StandardButton response =  QMessageBox::question(this, tr("QuitWithoutSave?"), tr("quit_without_save_info?"),
        QMessageBox::Ok| QMessageBox::Cancel);
    if (response == QMessageBox::Ok)
	{
		AppNetwork->execQueryByTemplate(QueryTemplates::unlockDocument, false, awaiter); 
		BarcodeObs->deactivate();
		emit backRequired();
	}
}

