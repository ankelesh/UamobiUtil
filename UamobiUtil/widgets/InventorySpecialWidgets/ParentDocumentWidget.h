#pragma once
#include "networking/RequestAwaiter.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/things.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/MultibranchWidgets/FilterSelectWidget.h"
namespace specwidgets
{
	class documentSelectionWidget : public AbstractListSelectionWidget
	{
		Q_OBJECT
	private:
		QVector<parsedDocument> & alldocs;
	protected:


		// Inherited via AbstractListSelectionWidget
		virtual int countElems() override;

		virtual void itemSelectedFromList(QListWidgetItem*) override;

		virtual QString elemToString(int);
	public:
		documentSelectionWidget(QVector<parsedDocument>& docs, QWidget* parent);
	signals:
		void docSelected(parsedDocument);
	};
}



class ParentDocumentWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVector<parsedDocument> alldocs;
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	MegaIconButton* filterButton;
	specwidgets::documentSelectionWidget* docSelection;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* selectButton;

	FilterSelectWidget* filterSelect;

	RequestAwaiter awaiter;
public:
	ParentDocumentWidget(GlobalAppSettings& go, QWidget* parent);
public slots:
	void loadDocuments();
	void filterDocuments();

protected slots:
	void load_documents_response();
	void was_timeout();
	void hideCurrent();
	void filterReady();
signals:
	void docSelected(parsedDocument);
};


