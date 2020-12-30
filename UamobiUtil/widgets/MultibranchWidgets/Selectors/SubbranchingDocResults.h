#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"





class SubbranchingDocResultsW : DocResultsWidget
{
	Q_OBJECT
protected:
	MegaIconButton* subbranchOpenButton;
	IndependentBranchNode* subbranch;

	virtual void _rewriteNames(QStringList& names) override;
public:
	SubbranchingDocResultsW(IndependentBranchNode* extra_branch, QWidget* parent = Q_NULLPTR);

protected slots:
	void openSubbranch();
};