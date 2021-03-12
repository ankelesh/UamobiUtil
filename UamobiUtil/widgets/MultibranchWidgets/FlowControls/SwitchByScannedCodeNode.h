#pragma once
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include "widgets/BranchingTools/AdjustableBranch.h"



class QLineEdit;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class MegaIconButton;

class SwitchByScannedCodeWidget : public IndependentBranchNode, abstractDynamicNode
{
	Q_OBJECT
protected:
	QVBoxLayout* innerLayout;
	QLabel* info;
	QLineEdit* barcodeInput;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* skipButton;
	MegaIconButton* okButton;

	IndependentBranchNode* normalFlowBranch;

	OverloadableQuery barcodeChecker;
	RecEntity prototype;
	BranchDescription descr;
	RequestAwaiter* awaiter;
	RecEntity toGiveToBranch;
	RecEntity pendingResult;
	// Inherited via IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;

	virtual void _sendDataRequest() override;
	virtual void _backReaction() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	virtual void setFocus() override;

public:
	SwitchByScannedCodeWidget(BranchDescription branch, QWidget* parent = Q_NULLPTR);


protected slots:
	void _innerBranchEnded(RecEntity);
	void _innerBranchBack();
	void _branchSkipped();
	void _timeout();
	void _parseResponse(QString, QString);
	void _barcodeReceived(QString);
	void _checkBarcode();
	void _confirmPressed();
};

