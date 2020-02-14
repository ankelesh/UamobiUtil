#include "ModeSelectionWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "legacy/legacy.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"



ModeSelectionWidget::ModeSelectionWidget( QWidget* parent)
	: inframedWidget(true, parent), innerModel(new DataEntityListModel(this)), mainLayout(new QVBoxLayout(this)),
	buttonLayout(new QHBoxLayout(this)), userTip(new QLabel(this)),
	modesTip(new QLabel(this)), modeSelection(new QListView(this)),
	logoutButton(new MegaIconButton(this)),settings(), 
	selected(new ModeEntity()), awaiter(AppSettings->timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(userTip);
	mainLayout->addWidget(modesTip);
	mainLayout->addWidget(modeSelection);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(logoutButton);
	//buttonLayout->addStretch();

	QFont scf = makeFont(0.04);
	userTip->setText(tr("mode_selection_user_tip!"));
	userTip->setAlignment(Qt::AlignCenter);
	userTip->setFont(scf);
	modesTip->setText(tr("mode_selection_modes_tip:"));
	modesTip->setAlignment(Qt::AlignCenter);
	modesTip->setFont(scf);

	logoutButton->setIcon(QIcon(":/res/back.png"));
	logoutButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	logoutButton->setText(tr("mode_selection_logout_tip"));

	modeSelection->setModel(innerModel);
	modeSelection->setItemDelegate(new ZebraItemDelegate(this));
#ifdef QT_VERSION5X
	QObject::connect(logoutButton, &QPushButton::clicked, this, &ModeSelectionWidget::logoutPressed);
	QObject::connect(modeSelection, &QListView::clicked, innerModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(innerModel, &DataEntityListModel::dataEntityClicked, this, &ModeSelectionWidget::modeSelected);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &ModeSelectionWidget::was_timeout);
#else
	QObject::connect(logoutButton, SIGNAL(clicked()), this, SLOT(logoutPressed()));
    QObject::connect(modeSelection, SIGNAL(clicked(QModelIndex)), innerModel, SLOT(mapClickToEntity(QModelIndex)));
	QObject::connect(innerModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(modeSelected(RecEntity)));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}


bool ModeSelectionWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
	if (val >= -1 && val <= innerModel->rowCount() - 1)
	{
		if (val == -1)
		{
			val = 9;
		}
		QModelIndex index = innerModel->index(val);
		if (index.isValid())
		{
			innerModel->mapClickToEntity(index);
			return true;
		}
		return true;
	}
	return false;
}

void ModeSelectionWidget::show()
{
	this->setFocus();
	inframedWidget::show();
}

void ModeSelectionWidget::loadModes()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ModeSelectionWidget::parse_modes);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_modes()));
#endif
	selected->sendAssociatedGetRequest(QStringList(), &awaiter);
}

void ModeSelectionWidget::logoutPressed()
{
	AppNetwork->execQueryByTemplate(QueryTemplates::LogOut, Q_NULLPTR);
	emit backRequired();
}

void ModeSelectionWidget::modeSelected(RecEntity Mode)
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ModeSelectionWidget::mode_select_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(mode_select_response()));
#endif
	selected = upcastRecord(RecEntity(Mode->clone()), selected);
	selected->sendAssociatedPostRequest(QStringList(), &awaiter);
}

void ModeSelectionWidget::hideCurrent()
{
	if (awaiter.isAwaiting())
		return;
	emit backRequired();
}

void ModeSelectionWidget::parse_modes()
{
	ResponseParser parser(new LinearListParser(awaiter.restext, awaiter.errtext));
	PolyResponse result = RequestParser::parseResponse(parser, RecEntity(selected->clone()));
	if (result.isError)
	{
		userTip->setText(result.errtext);
	}
	else
	{
		innerModel->setData(result.objects);
	}
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ModeSelectionWidget::mode_select_response()
{
	SimpliestResponceParser parser(awaiter.restext, awaiter.errtext);
	if (!parser.isSuccessfull())
	{
		userTip->setText(parser.getErrors());
		selected->drop();
	}
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
	emit modeAcquired(QHash<QString, QString>(), Mode(new ModeEntity(*selected)));
}

void ModeSelectionWidget::was_timeout()
{
	userTip->setText(tr("mode_selection_timeout!") + QString::number(AppSettings->timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived), 0, 0);
	hideProcessingOverlay();
}
