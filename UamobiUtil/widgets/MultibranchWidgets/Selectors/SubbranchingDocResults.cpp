#include "SubbranchingDocResults.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/MegaIconButton.h"



void SubbranchingDocResultsW::_rewriteNames(QStringList& names)
{
	if (names.count() > 0)
	{
		subbranchOpenButton->setText(names.last());
		names.pop_back();
	}
	DocResultsWidget::changeNames(names);
}

SubbranchingDocResultsW::SubbranchingDocResultsW(IndependentBranchNode* extra_branch, QWidget* parent)
	: DocResultsWidget(parent), subbranchOpenButton(new MegaIconButton(this)), subbranch(extra_branch)
{
	if (extra_branch == Q_NULLPTR)
	{
		subbranchOpenButton->hide();
	}
	else
	{
		extra_branch->setParent(this);
		mainLayout->addWidget(extra_branch);
		extra_branch->hide();
#ifdef QT_VERSION5X
		QObject::connect(subbranchOpenButton, &MegaIconButton::clicked, this, &SubbranchingDocResultsW::openSubbranch);
		QObject::connect(subbranch, &IndependentBranchNode::done, this, &SubbranchingDocResultsW::hideCurrent);
		QObject::connect(subbranch, &IndependentBranchNode::backRequired, this, &SubbranchingDocResultsW::hideCurrent);
#else
		QObject::connect(subbranchOpenButton, SIGNAL(clicked()), this, SLOT(openSubbranch()));
		QObject::connect(subbranch, SIGNAL(done()), this, SLOT(hideCurrent()));
		QObject::connect(subbranch, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	}
	subbranchOpenButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	footerLayout->insertWidget(1, subbranchOpenButton);
	subbranchOpenButton->setIcon(QIcon(":/res/stats.png"));
	subbranchOpenButton->setText(tr("Extra"));
}

void SubbranchingDocResultsW::openSubbranch()
{
	_hideAny(subbranch);
}
