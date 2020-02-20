#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QListView>
#endif
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/RequestAwaiter.h"

/*
	This widget is joining two polymorthic entities to use them for new selection.
	This widget can not work without valid dependency entity from _handleRecord.
	It uses dependency id and get-query of prototype to get list of prototyped type, then
	uses post-query to get richdata to fill text parameter of Order entity. This is
	rude behaviuor, it will be fixed later to ensure that any dependency and any prototype 
	can be used in it.
*/
using QueryTemplates::QueryCache;
class IdDependentSelectWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:
	RecEntity dependency;
	RecEntity prototype;

	DataEntityListModel* entityModel;

	QVBoxLayout* mainLayout;
	QLabel* userInfo;
	QListView* selectionView;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* pickButton;

	RequestAwaiter* awaiter;

	RecEntity awaitsConfirmation;

	QueryCache localCache;

	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	IdDependentSelectWidget(RecEntity proto, QWidget* = Q_NULLPTR);

	virtual bool isExpectingControl(int) override;
	// same as _handleRecord, renundant
	void loadData(RecEntity dependency);
protected slots:
	// sends associated post request of prototype using it's id
	void pickClicked();
	// same as pick clicked, but reacts on model click instead of current
	virtual void itemSelected(RecEntity);
	// parses response as list, extracting polyentities of type prototype
	void parse_get_response();
	// parses select response as it was richtext. !! HARD BIND TO OrderEntity !!
	void parse_select_response();
	void was_timeout();
};
