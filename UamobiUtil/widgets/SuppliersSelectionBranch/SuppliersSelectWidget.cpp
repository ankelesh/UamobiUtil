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

SuppliersSelectWidget::SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), allsuppliers(),
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new QWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
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
	ordfilterButton->setText(tr("W\O"));
	backButton->setText(tr("suppliers_selection_back"));

	ordfilterButton->setCheckable(true);
	ordfilterButton->setChecked(false);

	loadSuppliers();

#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SuppliersSelectWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SuppliersSelectWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SuppliersSelectWidget::backRequired);
	QObject::connect(supplierSelection, &specwidgets::_SupplierSelectionWidget::supplierPicked, this, &SuppliersSelectWidget::supplierPicked);
#else
	!!!implement!!!
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

void SuppliersSelectWidget::supplierPicked(parsedSupplier)
{
}

void SuppliersSelectWidget::loadSuppliers()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->recListSuppliers(userinputField->text(), ordfilterButton->isChecked(), &awaiter, RECEIVER_SLOT_NAME);
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
		allsuppliers = RequestParser::interpretAsSupplierList(awaiter.restext, awaiter.errtext);
		supplierSelection->reload();
	}
}
