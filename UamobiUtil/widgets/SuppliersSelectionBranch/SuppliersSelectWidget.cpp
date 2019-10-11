#include "SuppliersSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"

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

	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userHelp);
	innerLayout->addLayout(headerLayout);
	innerLayout->addWidget(supplierSelection);
	innerLayout->addLayout(footerLayout);
	headerLayout->addWidget(userinputField);
	headerLayout->addWidget(searchButton);
	headerLayout->addWidget(ordfilterButton);
	footerLayout->addWidget(backButton);
	//footerLayout->addStretch();

	userHelp->setText(tr("suppliers_selection_widget_user_tip"));
	userHelp->setStyleSheet(countAdaptiveFont(0.03));
	userHelp->setAlignment(Qt::AlignCenter);

	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setText(tr("suppliers_selection_search!"));
	searchButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
	ordfilterButton->setMinimumWidth(calculateAdaptiveWidth(0.2));

	userinputField->setStyleSheet(countAdaptiveFont(0.04));
	userinputField->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));

	supplierSelection->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

	backButton->setText(tr("suppliers_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	ordfilterButton->setText(tr("W\\O"));
	ordfilterButton->setCheckable(true);
	ordfilterButton->setChecked(true);
	ordFilterSwitched(true);


	loadSuppliers();

#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SuppliersSelectWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SuppliersSelectWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SuppliersSelectWidget::backRequired);
	QObject::connect(supplierSelection, &specwidgets::_SupplierSelectionWidget::supplierPicked, this, &SuppliersSelectWidget::supplierPicked);
	QObject::connect(userinputField, &QLineEdit::editingFinished, this, &SuppliersSelectWidget::searchPrimed);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(supplierSelection, SIGNAL(supplierPicked(parsedSupplier)), this, SLOT(supplierPicked(parsedSupplier)));
#endif
}

void SuppliersSelectWidget::show()
{
	setFocus();
	inframedWidget::show();
}

bool SuppliersSelectWidget::isExpectingControl(int val)
{
	if (val >= -1 && val < allsuppliers.count() - 1)
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
		supplierPicked(	allsuppliers.at(val));
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
	emit supplierAcquired(supp);
}

void SuppliersSelectWidget::loadSuppliers()
{
	if (awaiter.isAwaiting())
		return;
	(*globalSettings.networkingEngine.*listSuppliers)(userinputField->text(), ordfilterButton->isChecked(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userHelp->setText(tr("suppliers_select_widget_timeout!"));
	}
	else
	{
		allsuppliers = interpreter(awaiter.restext, awaiter.errtext);
		supplierSelection->reload();
	}
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