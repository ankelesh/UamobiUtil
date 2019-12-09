#include "InventoryRootWidget.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

void InventoryRootWidget::processOptions()
{
}

void InventoryRootWidget::openCorrespondingSubbranch()
{
	if (modeItself.submode.contains("partial", Qt::CaseInsensitive))
	{
		bfilterWidget = new BarcodeFilterSelectionSubbranch(globalSettings, this);
		mainLayout->addWidget(bfilterWidget);
#ifdef QT_VERSION5X
        QObject::connect(bfilterWidget, &BarcodeFilterSelectionSubbranch::selectionHappened, this, &InventoryRootWidget::continueToScaning);
		QObject::connect(bfilterWidget, &BarcodeFilterSelectionSubbranch::backRequired, this, &InventoryRootWidget::backTo);
#else
        QObject::connect(bfilterWidget, SIGNAL(backRequired()), this, SLOT(backTo()));
        QObject::connect(bfilterWidget, SIGNAL(selectionHappened()), this, SLOT(continueToScaning()));
#endif
		bfilterWidget->hide();
	}
	_hideAny(docSelectionWidget);
	docSelectionWidget->loadDocuments();
}

InventoryRootWidget::InventoryRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings, parsedMode mode, QWidget* parent)
	: inframedWidget(parent), abstractNode(), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	innerWidget(new InventoryParamsWidget(this)),
	docSelectionWidget(new ParentDocumentWidget(globalSettings, this)), scaningWidget(new InventoryScaningWidget(go, this)),
	bfilterWidget(),
	options(settings), modeItself(mode)
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	processOptions();
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(docSelectionWidget);
	mainLayout->addWidget(scaningWidget);
	innerWidget->hide();
	docSelectionWidget->hide();
	scaningWidget->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	openCorrespondingSubbranch();
	removeEventFilter(keyfilter);
#ifdef QT_VERSION5X

	QObject::connect(innerWidget, &InventoryParamsWidget::backRequired, this, &InventoryRootWidget::backTo);
	QObject::connect(innerWidget, &InventoryParamsWidget::documentConfirmed, this, &InventoryRootWidget::continueToScaning);
	QObject::connect(innerWidget, &InventoryParamsWidget::documentMustBeSelected, this, &InventoryRootWidget::backToStep);
	QObject::connect(docSelectionWidget, &ParentDocumentWidget::docSelected, this, &InventoryRootWidget::documentAcquired);
	QObject::connect(docSelectionWidget, &ParentDocumentWidget::backRequired, this, &InventoryRootWidget::backRequired);
	QObject::connect(scaningWidget, &InventoryScaningWidget::backRequired, this, &InventoryRootWidget::hideCurrent);
	QObject::connect(scaningWidget, &InventoryScaningWidget::saveSuccess, this, &InventoryRootWidget::hideCurrent);
#else
	QObject::connect(innerWidget, SIGNAL(backRequired()), this, SLOT(backTo()));
    QObject::connect(innerWidget, SIGNAL(documentConfirmed()), this, SLOT(continueToScaning()));
	QObject::connect(innerWidget, SIGNAL(documentMustBeSelected(int)), this, SLOT(backToStep(int)));
	QObject::connect(docSelectionWidget, SIGNAL(docSelected(parsedDocument)), this, SLOT(documentAcquired(parsedDocument)));
	QObject::connect(docSelectionWidget, SIGNAL(backRequired()), this, SIGNAL(backRequired()));
	QObject::connect(scaningWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(scaningWidget, SIGNAL(saveSuccess()), this, SLOT(hideCurrent()));
#endif
}

void InventoryRootWidget::documentAcquired(parsedDocument doc)
{
#ifdef DEBUG
	detrace_METHEXPL("creating doc from " << doc.description());
#endif
	Document Doc;
	Doc.comment = doc.text;
	Doc.docId = doc.code;
	innerWidget->setDocument(Doc);
	if (bfilterWidget != Q_NULLPTR)
	{
		_hideAny(bfilterWidget);
		return;
	}
	_hideCurrent(innerWidget);
}

void InventoryRootWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void InventoryRootWidget::backTo()
{
	_hideAny(docSelectionWidget);
	docSelectionWidget->loadDocuments();
}

void InventoryRootWidget::backToStep(int step)
{
	switch (step)
	{
	case steps::parentSelection:
		_hideAny(docSelectionWidget);
		break;
	case steps::Scaning:
		_hideAny(scaningWidget);
		break;
	default:
		return;
	}
}

void InventoryRootWidget::continueToScaning()
{
	_hideAny(scaningWidget);
	scaningWidget->setDocument(innerWidget->getDoc());
	scaningWidget->setModeName(modeItself.name);
}
