#pragma once
#include "widgets/BranchingTools/AbsBranch.h"
#include <QBoxLayout>
#include <QListWidget>
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"

/*
This subbranch is required for flow control of application. It shows some buttons to user, so he can choose
what to do. Entity given to this node is transmitted to subbranch.
*/


class SwitchSubbranch : public IndependentBranchNode, abstractDynamicNode
{
// don't know why android armv7a build hates this definition (armv8a likes it). If compilation error there, put Q_OBJECT
// in ifndef
#ifndef Q_OS_ANDROID
#endif
    Q_OBJECT
private:
	// description of this branch and subbranch. 
	BranchDescription root;

    // interface for picking
	QVBoxLayout* innerLayout;
	QListWidget* namePicking;
	MegaIconButton* backButton;
	RecEntity toGive;

	// inherited from IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	SwitchSubbranch(BranchDescription newRoot, QWidget* parent = Q_NULLPTR);
protected slots:
	// two version of slots because of changed signature
#ifdef QT_VERSION5X
	void itemClicked(const QModelIndex& ind);
#else
	void itemClicked(QModelIndex);
#endif
	void hideCurrent();
};
