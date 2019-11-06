#include "SuppliersSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#define DEBUG_FILTER
#ifdef DEBUG
#include "debugtrace.h"
#endif

QString specwidgets::_SupplierSelectionWidget::elemToString(int i)
{
	return suppliers.at(i).name;
}

int specwidgets::_SupplierSelectionWidget::countElems()
{
	return suppliers.count();
}

specwidgets::_SupplierSelectionWidget::_SupplierSelectionWidget(QVector<parsedSupplier>& supplist, QWidget* parent)
	: AbstractListSelectionWidget(parent), suppliers(supplist)
{
	init();
}

void specwidgets::_SupplierSelectionWidget::itemSelectedFromList(QListWidgetItem*)
{
	emit supplierPicked(suppliers.at(currentRow()));
}

SuppliersSelectWidget::SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent,
	SuppliersLikeMP meth, interpretsPointers::interpretAsSupplierLike inter)
	: inframedWidget(parent), globalSettings(go), allsuppliers(),
	listSuppliers(meth), interpreter(inter),
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	headerLayout(new QHBoxLayout(innerWidget)), footerLayout(new QHBoxLayout(innerWidget)),
	userHelp(new QLabel(innerWidget)), userinputField(new QLineEdit(innerWidget)),
	searchButton(new MegaIconButton(innerWidget)), ordfilterButton(new MegaIconButton(innerWidget)),
	supplierSelection(new specwidgets::_SupplierSelectionWidget(allsuppliers, innerWidget)),
	backButton(new MegaIconButton(innerWidget)), withOrd(":/res/with.png"), withoutOrd(":/res/without.png"),
	awaiter(go.timeoutInt, this)
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
	innerLayout->addWidget(supplierSelection);
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
	supplierSelection->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
	supplierSelection->setMaximumWidth(calculateAdaptiveWidth(1));

	backButton->setText(tr("suppliers_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	ordfilterButton->setText(tr("W\\O"));
	ordfilterButton->setCheckable(true);
	ordfilterButton->setChecked(true);
	ordFilterSwitched(true);
	loadSuppliers();
	supplierSelection->installEventFilter(keyfilter);
	innerWidget->installEventFilter(keyfilter);
#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SuppliersSelectWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SuppliersSelectWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SuppliersSelectWidget::backRequired);
	QObject::connect(supplierSelection, &specwidgets::_SupplierSelectionWidget::supplierPicked, this, &SuppliersSelectWidget::supplierPicked);
	QObject::connect(userinputField, &QLineEdit::editingFinished, this, &SuppliersSelectWidget::searchPrimed);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &SuppliersSelectWidget::was_timeout);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(supplierSelection, SIGNAL(supplierPicked(parsedSupplier)), this, SLOT(supplierPicked(parsedSupplier)));
	QObject::connect(userinputField, SIGNAL(editingFinished()), this, SLOT(searchPrimed()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void SuppliersSelectWidget::show()
{
	inframedWidget::show();
	setFocus();
}

bool SuppliersSelectWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
	if (val >= -1 && val <= allsuppliers.count() - 1)
	{
		if (val == -1)
		{
			if (allsuppliers.count() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		supplierPicked(allsuppliers.at(val));
		return true;
	}

	return false;
}

void SuppliersSelectWidget::searchPrimed()
{
	if (awaiter.isAwaiting())
		return;
	loadSuppliers();
}

void SuppliersSelectWidget::ordFilterSwitched(bool state)
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

void SuppliersSelectWidget::supplierPicked(parsedSupplier supp)
{
	if (awaiter.isAwaiting())
		return;
	confirmedSupplier = supp;
	hideProcessingOverlay();
	emit supplierAcquired(supp);
}

void SuppliersSelectWidget::parse_response()
{
#ifdef DEBUG
	detrace_METHCALL("parse_response");
#endif
	allsuppliers = interpreter(awaiter.restext, awaiter.errtext);
#ifdef DEBUG_FILTER
	parse_uniresults_functions::supplierResponse temp;
	for (int i = 0; i < allsuppliers.count(); ++i)
	{
		if (allsuppliers.at(i).name.startsWith("f", Qt::CaseInsensitive))
		{
			temp << allsuppliers.at(i);
		}
	}
	allsuppliers = temp;
#endif
	supplierSelection->reload();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void SuppliersSelectWidget::was_timeout()
{
#ifdef DEBUG
	detrace_METHCALL("WasTimeout");
#endif

	userHelp->setText(tr("suppliers_select_widget_timeout!") + QString::number(globalSettings.timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void SuppliersSelectWidget::loadSuppliers()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &SuppliersSelectWidget::parse_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_response()));
#endif
	(*globalSettings.networkingEngine.*listSuppliers)(userinputField->text(), ordfilterButton->isChecked(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

SuppliersSelectionBranch::SuppliersSelectionBranch(GlobalAppSettings& go, QWidget* parent, SuppliersLikeMP meth,
	interpretsPointers::interpretAsSupplierLike inter)
	: SuppliersSelectWidget(go, parent, meth, inter), abstractNode(),
	orderSelection(new OrderSelectionWidget(go, confirmedSupplier, this))
{
	mainLayout->addWidget(orderSelection);
	orderSelection->hide();

	current = innerWidget;
	untouchable = innerWidget;
	main = this;
#ifdef QT_VERSION5X
	QObject::connect(orderSelection, &OrderSelectionWidget::orderConfirmed, this, &SuppliersSelectionBranch::orderAcquired);
	QObject::connect(orderSelection, &OrderSelectionWidget::backRequired, this, &SuppliersSelectionBranch::hideCurrent);
#else
	QObject::connect(orderSelection, SIGNAL(orderConfirmed(parsedOrder, QString)), this, SLOT(orderAcquired(parsedOrder)));
	QObject::connect(orderSelection, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

void SuppliersSelectionBranch::show()
{
	inframedWidget::show();
	supplierSelection->setFocus();
}

void SuppliersSelectionBranch::hideCurrent()
{
	if (current == innerWidget)
	{
		emit backRequired();
	}
	else
	{
		_hideCurrent(innerWidget);
	}
}

void SuppliersSelectionBranch::supplierPicked(parsedSupplier ps)
{
#ifdef DEBUG
	//detrace_METHCALL("supplier picked");
#endif
	SuppliersSelectWidget::supplierPicked(ps);
	_hideAny(orderSelection);
	orderSelection->loadOrders();
}

void SuppliersSelectionBranch::orderAcquired(parsedOrder po)
{
}