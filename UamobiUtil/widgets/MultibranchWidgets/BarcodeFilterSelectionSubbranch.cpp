#include "BarcodeFilterSelectionSubbranch.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void BarcodeFilterSelectionSubbranch::_handleRecord(RecEntity e)
{
	if (e->myType() == UniformXmlObject::Document)
	{
		FullDocument temp = upcastRecord<FullDocumentEntity>(e);
		doc = temp;
		assertAndShow(temp->parentNr);
	}
}

BarcodeFilterSelectionSubbranch::BarcodeFilterSelectionSubbranch(QWidget* parent)
	: IndependentBranchNode(true, parent), mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), currentFilterInfo(new QLabel(innerWidget)),
	stillageButton(new MegaIconButton(innerWidget)), suppliersButton(new MegaIconButton(innerWidget)),
	groupButton(new MegaIconButton(innerWidget)), nofilterButton(new MegaIconButton(innerWidget)),
	passButton(new MegaIconButton(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	supplierWidget(new SelectItemFromListWidget(this, RecEntity(new SupplierEntity()))),
	stillageWidget(new SelectItemFromListWidget(this, RecEntity(new StillageEntity()))),
	groupWidget(new SelectItemFromListWidget(this, RecEntity(new GroupEntity()))),
	awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(currentFilterInfo);
	innerLayout->addWidget(stillageButton);
	innerLayout->addWidget(suppliersButton);
	innerLayout->addWidget(groupButton);
	innerLayout->addWidget(nofilterButton);
	innerLayout->addWidget(passButton);
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
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	QSizePolicy qsp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    stillageButton->setIcon(QIcon(":/res/stillages.png"));
	stillageButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	stillageButton->setText(tr("stillages"));
	stillageButton->setSizePolicy(qsp);

	suppliersButton->setIcon(QIcon(":/res/suppliers.png"));
	suppliersButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	suppliersButton->setText(tr("suppliers"));
	suppliersButton->setSizePolicy(qsp);

    groupButton->setIcon(QIcon(":/res/groups.png"));
	groupButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	groupButton->setText(tr("groups"));
	groupButton->setSizePolicy(qsp);

	nofilterButton->setIcon(QIcon(":/res/data.png"));
	nofilterButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	nofilterButton->setText(tr("nofilter"));
	nofilterButton->setSizePolicy(qsp);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
	backButton->setSizePolicy(qsp);
	passButton->setIcon(QIcon(":/res/forward.png"));
	passButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	passButton->setText(tr("OK"));
	passButton->setSizePolicy(qsp);
	passButton->hide();
	currentFilterInfo->setAlignment(Qt::AlignCenter);
	currentFilterInfo->setFont(makeFont(0.04));
#ifdef QT_VERSION5X
	QObject::connect(stillageButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::stillageSelectRequired);
	QObject::connect(groupButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::groupSelectRequired);
	QObject::connect(suppliersButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::supplierSelectRequired);
	QObject::connect(nofilterButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::noFilterRequired);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::backRequired);
	QObject::connect(supplierWidget, &SelectItemFromListWidget::done, this, &BarcodeFilterSelectionSubbranch::filterSelected);
	QObject::connect(supplierWidget, &SelectItemFromListWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);
	QObject::connect(groupWidget, &SelectItemFromListWidget::done, this, &BarcodeFilterSelectionSubbranch::filterSelected);
	QObject::connect(groupWidget, &SelectItemFromListWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);
	QObject::connect(stillageWidget, &SelectItemFromListWidget::done, this, &BarcodeFilterSelectionSubbranch::filterSelected);
	QObject::connect(stillageWidget, &SelectItemFromListWidget::backRequired, this, &BarcodeFilterSelectionSubbranch::hideCurrent);
	QObject::connect(passButton, &MegaIconButton::clicked, this, &BarcodeFilterSelectionSubbranch::noFilterRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &BarcodeFilterSelectionSubbranch::got_response);

#else
	//NONFIXED
    QObject::connect(stillageButton, &MegaIconButton::clicked, this, SLOT(stillageSelectRequired()));
    QObject::connect(groupButton, SIGNAL(clicked()), this, SLOT(groupSelectRequired()));
    QObject::connect(suppliersButton, SIGNAL(clicked()), this, SLOT(supplierSelectRequired()));
    QObject::connect(nofilterButton, SIGNAL(clicked()), this, SLOT(noFilterRequired()));
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
    QObject::connect(supplierWidget, SIGNAL(supplierAcquired(parsedSupplier)), this, SLOT(supplierProxyWrapper(parsedSupplier)));
    QObject::connect(supplierWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(groupWidget, SIGNAL(done(QString,QString)), this, SLOT(applyFilter(QString,QString)));
    QObject::connect(groupWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(stillageWidget, SIGNAL(done(QString,QString)), this, SLOT(applyFilter(QString,QString)));
    QObject::connect(stillageWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(passButton, SIGNAL(clicked()), this, SIGNAL(selectionHappened()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(got_response()));
#endif
}

void BarcodeFilterSelectionSubbranch::assertAndShow(QString &pdoc)
{
	showProcessingOverlay();
	AppNetwork->execQueryByTemplate(QueryTemplates::getFilterItem, pdoc, awaiter);
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


void BarcodeFilterSelectionSubbranch::noFilterRequired()
{
	applyFilter("", "");
}

void BarcodeFilterSelectionSubbranch::filterSelected(RecEntity e)
{
	switch (e->myType())
	{
	case UniformXmlObject::Group:
		applyFilter(upcastRecord<GroupEntity>(e)->id, "group");
		break;
	case UniformXmlObject::Supplier:
		applyFilter(upcastRecord<SupplierEntity>(e)->code, "supplier");
		break;
	case UniformXmlObject::Stillage:
		applyFilter(upcastRecord<StillageEntity>(e)->id, "stillage");
		break;
	default:
#ifdef DEBUG
		detrace_METHPERROR("BarcodeFilterSelectionSubbranch::filterSelected",
			"entity can not be casted: type " << e->myType());
#endif // DEBUG
		break;
	}

}

void BarcodeFilterSelectionSubbranch::applyFilter(QString param, QString type)
{
	_hideAny(innerWidget);
	AppNetwork->execQueryByTemplate(QueryTemplates::applyBarcodeFilter, type, param, awaiter);
	emit done(doc);
}

void BarcodeFilterSelectionSubbranch::got_response()
{
	hideProcessingOverlay();
	
	if (awaiter->restext.contains(QLatin1String("all")))
	{
		groupButton->setDisabled(false);
		stillageButton->setDisabled(false);
		suppliersButton->setDisabled(false);
		nofilterButton->setDisabled(false);
		passButton->setDisabled(true);
		passButton->hide();
		currentFilterInfo->clear();
		show();
	}
	else if (awaiter->restext.contains(QLatin1String("none")))
	{
		emit done(doc);
	}
	else
	{
		groupButton->setDisabled(true);
		stillageButton->setDisabled(true);
		suppliersButton->setDisabled(true);
		nofilterButton->setDisabled(true);
		nofilterButton->hide();
		passButton->setDisabled(false);
		passButton->show();
		QStringList temp = awaiter->restext.split("<show>");
		if (temp.length() > 1)
		{
			temp = temp.at(1).split("</show>");
		}
		if (temp.count() > 0)
		{
			currentFilterInfo->setText(temp.at(0));
		}
		show();
	}
}

void BarcodeFilterSelectionSubbranch::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void BarcodeFilterSelectionSubbranch::_sendDataRequest()
{
}

