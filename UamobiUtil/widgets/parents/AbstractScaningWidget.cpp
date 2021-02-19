#include "AbstractScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "ScaningCore/BarcodeObserver.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"

#include "widgets/BranchingTools/BranchException.h"

void AbstractScaningWidget::_backReaction()
{
	auto t = sender();
	backNeeded();
}

void AbstractScaningWidget::useControls(const QVector<QSharedPointer < InputControlEntity> >& cvals)
{
    switch (cvals.count())
	{
	case 0:
		switch (controlsAvailable)
		{
		case 2:
            controlPanel->removeWidget(second_control->myWidget());
			second_control.clear();
            Q_FALLTHROUGH();
		case 1:
            controlPanel->removeWidget(first_control->myWidget());
			first_control.clear();
            Q_FALLTHROUGH();
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
			if (first_control->myType() == cvals.at(0)->type)
			{
                first_control->setValue(cvals.at(0)->defaultValue);
				first_control->show();
			}
			else
			{
                controlPanel->removeWidget(first_control->myWidget());
				first_control.clear();
                first_control = Control(fabricateControl(cvals.at(0), controlPanel, innerWidget));
			}
			if (!second_control.isNull())
			{
                controlPanel->removeWidget(second_control->myWidget());
				second_control.clear();
				--controlsAvailable;
			}
		}
		else
		{
            first_control = Control(fabricateControl(cvals.at(0), controlPanel, innerWidget));
			++controlsAvailable;
		}
		first_control->show();
		controlsRequired = true;
		submitButton->setDisabled(false);
		controlsAvailable = 1;
		return;
	case 2:
	default:
		switch (controlsAvailable)
		{
		case 2:
			if (!(cvals.at(1)->type == second_control->myType()))
			{
                controlPanel->removeWidget(second_control->myWidget());
				second_control.clear();
			}
            Q_FALLTHROUGH();
		case 1:
			if (!(cvals.at(1)->type == first_control->myType()))
			{
                controlPanel->removeWidget(first_control->myWidget());
				first_control.clear();
			}
            Q_FALLTHROUGH();
		case 0:
			if (first_control.isNull())
			{
                first_control = Control(fabricateControl(cvals.at(0), innerLayout, innerWidget));
			}
			else
			{
				first_control->reset();
			}
			if (second_control.isNull())
			{
                second_control = Control(fabricateControl(cvals.at(1), innerLayout, innerWidget));
			}
			else
			{
                second_control->setValue(cvals.at(1)->defaultValue);
			}
		}
		controlsAvailable = 2;
	}
	controlsRequired = true;
	submitButton->setDisabled(false);
	first_control->show();
	second_control->show();
}

void AbstractScaningWidget::wipe()
{
	barcodeField->clear();
	
}

void AbstractScaningWidget::setDocument(RecEntity e)
{
#ifdef DEBUG
	detrace_METHCALL("abstract method setDocument called with downcasted entity " << e->myType() << " " << e->getId());
#endif
}




AbstractScaningWidget::AbstractScaningWidget(int Id, QWidget* parent)
    : IndependentBranchNode(Id, parent), document(new FullDocumentEntity()),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), topPanelLayout(new QHBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)),
	barcodeField(new QLineEdit(innerWidget)),
	mainTextView(new QTextEdit(innerWidget)),
    controlPanel(new QVBoxLayout(innerWidget)),
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
	
	topPanelLayout->addWidget(userInfo);
	innerLayout->addWidget(barcodeField);

#ifdef Q_OS_ANDROID
    innerLayout->addLayout(controlPanel);
	innerLayout->addWidget(mainTextView);
#else
    innerLayout->addWidget(mainTextView);
    innerLayout->addLayout(controlPanel);
#endif

	topPanelLayout->addWidget(quitButton);
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

	userInfo->setText(tr("scaning_widget_user_info"));
	userInfo->setAlignment(Qt::AlignCenter);
#ifdef Q_OS_WINCE
	userInfo->setFont(QFont("Arial", 7,7));
#else
#ifdef Q_OS_WIN
	userInfo->setFont(QFont("Arial", 9, 9));
#else
    userInfo->setFont(*AppFonts->general());
#endif
#endif

	userInfo->setWordWrap(true);
    userInfo->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));

	mainTextView->setText(tr("scaning_widget_filler_text"));
	mainTextView->setFont(FontAdapter::makeFont(0.5));
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
    quitButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
    quitButton->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));

	barcodeField->setFocus();
	barcodeField->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));
	barcodeField->setFont(AppFonts->makeFont(1.5));
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
    ErrorMessageDialog::showErrorInfo(tr("Timeout"), tr("Critical timeout, can not continue"));
    hideProcessingOverlay();
    if (awaiter->deliverHere(receiptNewDocument))
        throwException(new BranchException(BranchException::ToModeSelection));
}

void AbstractScaningWidget::quitNoSave()
{
	QMessageBox::StandardButton response =  QMessageBox::question(this, tr("QuitWithoutSave?"), tr("quit_without_save_info?"),
        QMessageBox::Ok| QMessageBox::Cancel);
    if (response == QMessageBox::Ok)
	{
        AppNetwork->execQueryByTemplate(QueryTemplates::unlockDocument, "false", Q_NULLPTR);
		BarcodeObs->deactivate();
		emit backRequired();
	}
}

