#include "BarcodeFilterSelectionSubbranch.h"


BarcodeFilterSelectionSubbranch::BarcodeFilterSelectionSubbranch(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), stillageButton(new MegaIconButton(innerWidget)), suppliersButton(new MegaIconButton(innerWidget)),
	groupButton(new MegaIconButton(innerWidget)), nofilterButton(new MegaIconButton(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	supplierWidget(new SuppliersSelectWidget(go, this, "suppliers", &RequestParser::interpretAsSupplierList, false)), stillageWidget(new StillageSelectionWidget(go, this)),
	groupWidget(new GroupSelectionWidget(go, this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(stillageButton);
	innerLayout->addWidget(suppliersButton);
	innerLayout->addWidget(groupButton);
	innerLayout->addWidget(nofilterButton);
	innerLayout->addWidget(backButton);
	mainLayout->addWidget(supplierWidget);
	mainLayout->addWidget(groupWidget);
	mainLayout->addWidget(stillageWidget);
	supplierWidget->hide();
	groupWidget->hide();
	stillageWidget->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;

	QSizePolicy qsp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	stillageButton->setIcon(QIcon(":/res/stillage.png"));
	stillageButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	stillageButton->setText(tr("stillages"));
	stillageButton->setSizePolicy(qsp);

	suppliersButton->setIcon(QIcon(":/res/suppliers.png"));
	suppliersButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	suppliersButton->setText(tr("suppliers"));
	suppliersButton->setSizePolicy(qsp);

	groupButton->setIcon(QIcon(":/res/group.png"));
	groupButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	groupButton->setText(tr("groups"));
	groupButton->setSizePolicy(qsp);

	nofilterButton->setIcon(QIcon(":/res/data.png"));
	nofilterButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	nofilterButton->setText(tr("nofilter"));
	nofilterButton->setSizePolicy(qsp);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
	backButton->setSizePolicy(qsp);

#ifdef QT_VERSION5X
	QObject::connect(stillageButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::stillageSelectRequired);
	QObject::connect(groupButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::groupSelectRequired);
	QObject::connect(suppliersButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::supplierSelectRequired);
	QObject::connect(nofilterButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::noFilterRequired);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::backRequired);
	QObject::connect(supplierWidget, &SuppliersSelectWidget::supplierAcquired, this, &BarcodeFilterSelectionSubbranch::supplierProxyWrapper);
	QObject::connect(supplierWidget, &SuppliersSelectWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);
	QObject::connect(groupWidget, &GroupSelectionWidget::done, this, &BarcodeFilterSelectionSubbranch::applyFilter);
	QObject::connect(groupWidget, &GroupSelectionWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);
	QObject::connect(stillageWidget, &StillageSelectionWidget::done, this, &BarcodeFilterSelectionSubbranch::applyFilter);
	QObject::connect(stillageWidget, &StillageSelectionWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);

#else
	!!!implement!!!
#endif
}

void BarcodeFilterSelectionSubbranch::supplierSelectRequired()
{
	_hideAny(supplierWidget);
	supplierWidget->loadItems();
}

void BarcodeFilterSelectionSubbranch::stillageSelectRequired()
{
	_hideAny(stillageWidget);
	stillageWidget->loadItems();
}

void BarcodeFilterSelectionSubbranch::groupSelectRequired()
{
	_hideAny(groupWidget);
	groupWidget->loadItems();
}

void BarcodeFilterSelectionSubbranch::supplierProxyWrapper(parsedSupplier ps)
{
	applyFilter(ps.code, "supplier");
}

void BarcodeFilterSelectionSubbranch::noFilterRequired()
{
	applyFilter("", "");
}

void BarcodeFilterSelectionSubbranch::applyFilter(QString param, QString type)
{
	globalSettings.networkingEngine->applyBarcodeFilter(type, param, Q_NULLPTR, "");
	emit selectionHappened();
}

void BarcodeFilterSelectionSubbranch::hideCurrent()
{
	_hideCurrent(innerWidget);
}

