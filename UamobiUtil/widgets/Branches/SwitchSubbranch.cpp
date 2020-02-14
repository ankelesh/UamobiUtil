#include "SwitchSubbranch.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/BranchingTools/BranchFactory.h"
void SwitchSubbranch::_handleRecord(RecEntity e)
{
	toGive = RecEntity( e->clone());
}

void SwitchSubbranch::_sendDataRequest()
{
}

void SwitchSubbranch::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
}

SwitchSubbranch::SwitchSubbranch(BranchDescription newRoot, QWidget* parent)
	: IndependentBranchNode(independent_nodes::Switch, parent), 
	abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
	root(newRoot),
	innerLayout(new QVBoxLayout(untouchable)), namePicking(new QListWidget(this)), 
	backButton(new MegaIconButton(this)), toGive()
{
	setLayout(mainLayout);
	mainLayout->addWidget(untouchable);
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(namePicking);
	innerLayout->addWidget(backButton);

	namePicking->setFont(*FontAdapter::general());
	namePicking->setSelectionMode(QListView::NoSelection);
	namePicking->setResizeMode(QListView::Adjust);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	int min = ((root->namesOverload.count() <= root->count()) ? root->namesOverload.count() : root->count());
	for (int i = 0; i < min; ++i)
	{
		namePicking->addItem(root->namesOverload.at(i));
	}
#ifdef QT_VERSION5X
	QObject::connect(backButton, &MegaIconButton::clicked, this, &IndependentBranchNode::backRequired);
	QObject::connect(namePicking, &QListWidget::clicked, this, &SwitchSubbranch::itemClicked);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(namePicking, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
#endif
}
#ifdef QT_VERSION5X
void SwitchSubbranch::itemClicked(const QModelIndex& ind)
{
	if (ind.isValid())
	{
		_hideAndDeleteCurrent(BranchNodeFactory::createNode(
			root->at(ind.row()),
			this
		));
		QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &SwitchSubbranch::hideCurrent);
		QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::done, this, &IndependentBranchNode::done);
		_upCO<IndependentBranchNode>()->loadData();
		_upCO<IndependentBranchNode>()->processRecord(toGive);
	}
}
#else
void SwitchSubbranch::itemClicked(QModelIndex ind)
{
	if (ind.isValid())
	{
		_hideAndDeleteCurrent(BranchNodeFactory::createNode(
			root->at(ind.row()),
			this
		));
		QObject::connect(currentlyOpened, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
        QObject::connect(_upCO<IndependentBranchNode>(), SIGNAL(done(RecEntity)), this, SIGNAL(done(RecEntity)));
		_upCO<IndependentBranchNode>()->loadData();
		_upCO<IndependentBranchNode>()->processRecord(toGive);
	}
}
#endif
void SwitchSubbranch::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}
