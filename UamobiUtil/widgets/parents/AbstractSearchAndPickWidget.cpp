#include "AbstractSearchAndPickWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"

void AbstractSearchAndPickWidget::emplaceSelection(AbstractListSelectionWidget* toEmplace)
{
#ifdef QT_VERSION5X
    innerLayout->insertWidget(1, toEmplace);
#else
    innerLayout->insertWidget(2, toEmplace);
#endif
	itemSelection = toEmplace;
	itemSelection->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
	itemSelection->setMaximumWidth(calculateAdaptiveWidth(1));
	itemSelection->installEventFilter(keyfilter);
}

AbstractSearchAndPickWidget::AbstractSearchAndPickWidget(
	GlobalAppSettings& go, QWidget* parent, QString iname, TemplatedListRequest meth,
	 bool isRequiredOrd)
	: inframedWidget(true,parent), globalSettings(go),
	listItems(meth), 
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	headerLayout(new QHBoxLayout(innerWidget)), footerLayout(new QHBoxLayout(innerWidget)),
	userHelp(new QLabel(innerWidget)), userinputField(new QLineEdit(innerWidget)),
	searchButton(new MegaIconButton(innerWidget)), ordfilterButton(new MegaIconButton(innerWidget)),
	itemSelection(),
	backButton(new MegaIconButton(innerWidget)), withOrd(":/res/with.png"), withoutOrd(":/res/without.png"),
	awaiter(go.timeoutInt, this), itemName(iname)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerWidget->setLayout(innerLayout);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->addWidget(userHelp);
	innerLayout->addLayout(headerLayout);
	innerLayout->addLayout(footerLayout);
	headerLayout->addWidget(userinputField);
	headerLayout->addWidget(searchButton);
	headerLayout->addWidget(ordfilterButton);
	footerLayout->addWidget(backButton);
	//footerLayout->addStretch();
	QFont scf = makeFont(0.04);
	userHelp->setText(tr("suppliers_selection_widget_user_tip"));
	userHelp->setFont(scf);
	userHelp->setAlignment(Qt::AlignCenter);

	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setText(tr("suppliers_selection_search!"));
	searchButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
	ordfilterButton->setMinimumWidth(calculateAdaptiveWidth(0.2));

	userinputField->setFont(scf);
	userinputField->setMaximumWidth(calculateAdaptiveWidth(0.6));
	userinputField->setMinimumWidth(calculateAdaptiveWidth(0.4));
	userinputField->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	userinputField->setMaximumHeight(calculateAdaptiveButtonHeight());

	backButton->setText(tr("suppliers_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	ordfilterButton->setText(tr("W\\O"));
	ordfilterButton->setCheckable(true);
	ordFilterSwitched(true);
	innerWidget->installEventFilter(keyfilter);
	if (!isRequiredOrd)
	{
		ordfilterButton->setChecked(false);
		ordfilterButton->hide();
	}
	else
	{
		ordfilterButton->setChecked(true);
	}
#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &AbstractSearchAndPickWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &AbstractSearchAndPickWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractSearchAndPickWidget::backRequired);
	QObject::connect(userinputField, &QLineEdit::editingFinished, this, &AbstractSearchAndPickWidget::searchPrimed);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &AbstractSearchAndPickWidget::was_timeout);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(userinputField, SIGNAL(editingFinished()), this, SLOT(searchPrimed()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void AbstractSearchAndPickWidget::show()
{
    loadItems();
	inframedWidget::show();
	setFocus();
}


void AbstractSearchAndPickWidget::searchPrimed()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &AbstractSearchAndPickWidget::parse_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_response()));
#endif
	(*globalSettings.networkingEngine.*listItems)(itemName, userinputField->text(), 
		((ordfilterButton->isChecked()) ? "hasorders=true" : "") , &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void AbstractSearchAndPickWidget::ordFilterSwitched(bool)
{
	if (ordfilterButton->isChecked())
	{
		ordfilterButton->setIcon(withOrd);
		ordfilterButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	}
	else
	{
		ordfilterButton->setIcon(withoutOrd);
		ordfilterButton->setStyleSheet(UNCHECKED_BUTTONS_STYLESHEET);
	}
}


void AbstractSearchAndPickWidget::parse_response()
{
	interpretResult();
	itemSelection->reload();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void AbstractSearchAndPickWidget::was_timeout()
{
	userHelp->setText(tr("suppliers_select_widget_timeout!") + QString::number(globalSettings.timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void AbstractSearchAndPickWidget::loadItems()
{
	searchPrimed();
}
