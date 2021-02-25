#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
// Qt 4 only imports
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>
#endif
#include "widgets/parents/IndependentBranchNode.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"

#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "networking/things.h"

/*
	This widget branch is allowing user to select barcode filter that will be sent to server.
	It allows to attach 3 nodes which must return entity with id to successfully finish branch.
	This widget is similar with switch widget, but has all functionality required prebuilt.
*/
using QueryTemplates::OverloadableQuery;
class BarcodeFilterSelectionSubbranch : public IndependentBranchNode, abstractNode
{
	Q_OBJECT
protected:

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel * currentFilterInfo;
	MegaIconButton* stillageButton;
	MegaIconButton* suppliersButton;
	MegaIconButton* groupButton;
	MegaIconButton* nofilterButton;
	MegaIconButton* passButton;
	MegaIconButton* backButton;

	// attachments. 
	IndependentBranchNode* supplierWidget;
	IndependentBranchNode* stillageWidget;
	IndependentBranchNode* groupWidget;
	RequestAwaiter* awaiter;

	RecEntity doc;

	// overloadable queries
	OverloadableQuery applyFilterQuery;
	OverloadableQuery getFiltersQuery;
	// Inherited via IndependentBranchNode
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _numberReaction(int) override;
public:
	// uses default subbranches if no subbranch provided
	BarcodeFilterSelectionSubbranch(QWidget* parent = Q_NULLPTR, IndependentBranchNode* supp = Q_NULLPTR,
		IndependentBranchNode* still = Q_NULLPTR, IndependentBranchNode* group = Q_NULLPTR
		);
	// sends getFilters to define what to show user
    void assertAndShow(const QString& pdoc);
	// applies filter, not requires record
	void applyFilter(QString param, QString type);
protected slots:
	// buttons reactions
	void supplierSelectRequired();
	void stillageSelectRequired();
	void groupSelectRequired();
	void noFilterRequired();
	// reaction to subbranch end
	void filterSelected(RecEntity);
	// awaiter reaction
	void got_response();
	void hideCurrent();



};
