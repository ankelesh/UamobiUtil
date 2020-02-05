#pragma once
#include "networking/RequestAwaiter.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#ifdef QT_VERSION5X
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#else
#include <QtGui/qboxlayout.h>
#include <QtGui/qlabel.h>
#endif
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/things.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/MultibranchWidgets/FilterSelectWidget.h"

/*
	This widget is used for selecting parent document. It differs from other selections because of
	filter subpage.

	__ASSOCIATED_DATABASE_FUNCTION__   :  P'documentsResponse' inv_list_parent_docs

*/

class ParentDocumentWidget : public IndependentBranchNode, abstractNode
{
	Q_OBJECT
protected:
	DataEntityListModel*  entityModel;
	RecEntity prototype;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	MegaIconButton* filterButton;
	QListView* docSelection;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* selectButton;

	FilterSelectWidget* filterSelect;

	RequestAwaiter* awaiter;
	virtual void _sendDataRequest() override;
public:
	ParentDocumentWidget(RecEntity proto, QWidget* parent);
public slots:
	void loadDocuments();
	void filterDocuments();

protected slots:
	void load_documents_response();
	void was_timeout();
	void hideCurrent();
	void filterReady();
};
