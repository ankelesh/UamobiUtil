#include "SuppliersSelectWidget.h"

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
	searchButton(new QPushButton(innerWidget)), ordfilterButton(new QPushButton(innerWidget)),
	supplierSelection(new specwidgets::_SupplierSelectionWidget(allsuppliers, innerWidget)),
	backButton(new QPushButton(innerWidget))
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
	footerLayout->addStretch();

	userHelp->setText(tr("suppliers_selection_widget_user_tip"));
	searchButton->setText(tr("suppliers_selection_search!"));
	ordfilterButton->setText(tr("W\\O"));
	backButton->setText(tr("suppliers_selection_back"));

	ordfilterButton->setCheckable(true);
	ordfilterButton->setChecked(true);

	loadSuppliers();

#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SuppliersSelectWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SuppliersSelectWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SuppliersSelectWidget::backRequired);
	QObject::connect(supplierSelection, &specwidgets::_SupplierSelectionWidget::supplierPicked, this, &SuppliersSelectWidget::supplierPicked);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(supplierSelection, SIGNAL(supplierPicked(parsedSupplier)), this, SLOT(supplierPicked(parsedSupplier)));
#endif
}

void SuppliersSelectWidget::searchPrimed()
{
	loadSuppliers();
}

void SuppliersSelectWidget::ordFilterSwitched(bool state)
{
	if (ordfilterButton->isChecked())
	{
		ordfilterButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	}
	else
	{
		ordfilterButton->setStyleSheet("");
	}
}

void SuppliersSelectWidget::supplierPicked(parsedSupplier supp)
{
	confirmedSupplier = supp;
	emit supplierAcquired(supp);
}

void SuppliersSelectWidget::loadSuppliers()
{
	RequestAwaiter awaiter;
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