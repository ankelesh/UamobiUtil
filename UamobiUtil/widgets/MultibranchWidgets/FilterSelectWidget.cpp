#include "FilterSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif

QString specwidgets::TypeCheckboxSelection::elemAsString(int index)
{
	return alltypes.at(index).name + " " + alltypes.at(index).id;
}

int specwidgets::TypeCheckboxSelection::countElems()
{
	return alltypes.count();
}

specwidgets::TypeCheckboxSelection::TypeCheckboxSelection(QVector<parsedDocType>& at, QVector<bool>& statesv, QWidget* parent)
	: AbstractCheckboxSelection(statesv, parent), alltypes(at)
{
}

FilterSelectWidget::FilterSelectWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), doctypes(),
	selectionState(), mainLayout(new QVBoxLayout(this)),
	title(new QLabel(this)), topPanelLayout(new QHBoxLayout(this)),
	allonButton(new MegaIconButton(this)), alloffButton(new MegaIconButton(this)),
	scrArea(new QScrollArea(this)),
	typesel(new specwidgets::TypeCheckboxSelection(doctypes, selectionState, scrArea)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), awaiter(go.timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(title);
	mainLayout->addLayout(topPanelLayout);
	topPanelLayout->setContentsMargins(0, 0, 0, 0);
	topPanelLayout->setSpacing(0);
	topPanelLayout->addWidget(allonButton);
	topPanelLayout->addWidget(alloffButton);
	mainLayout->addWidget(scrArea);
	scrArea->setWidgetResizable(true);
	typesel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	mainLayout->addLayout(footerLayout);
	footerLayout->setContentsMargins(0, 0, 0, 0);
	footerLayout->setSpacing(0);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);
	QFont scf = makeFont(0.04);
	title->setFont(scf);
	title->setAlignment(Qt::AlignCenter);
	title->setText(tr("filter_widget_title"));
	allonButton->setText(tr("all_on_button"));
	allonButton->setIcon(QIcon(":/res/single.png"));
	allonButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	alloffButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	alloffButton->setIcon(QIcon(":/res/data.png"));
	alloffButton->setText(tr("all_off_button"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setText(tr("filter!"));
	okButton->setIcon(QIcon(":/res/filter.png"));
	okButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET); 

	QScroller::grabGesture(scrArea, QScroller::LeftMouseButtonGesture);
#ifdef QT_VERSION5X

	QObject::connect(allonButton, &MegaIconButton::clicked, this, &FilterSelectWidget::checkAll);
	QObject::connect(alloffButton, &MegaIconButton::clicked, this, &FilterSelectWidget::uncheckAll);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &FilterSelectWidget::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &FilterSelectWidget::okPressed);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &FilterSelectWidget::was_timeout);
	
#else
	!!!implement!!!
#endif
		scrArea->setWidget(typesel);
}

void FilterSelectWidget::loadFilters()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &FilterSelectWidget::parse_doctype_list_response);
#else
	!!!implement!!!
#endif
	globalSettings.networkingEngine->docGetAllowedTypes(&awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void FilterSelectWidget::checkAll()
{
	for (int i = 0; i < selectionState.count(); ++i)
	{
		selectionState[i] = true;
	}
	typesel->reload();
}

void FilterSelectWidget::uncheckAll()
{
	for (int i = 0; i < selectionState.count(); ++i)
	{
		selectionState[i] = false;
	}
	typesel->reload();
}

void FilterSelectWidget::okPressed()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &FilterSelectWidget::parse_doctype_selection_response);
#else
	!!!implement!!!
#endif
		QString buffer;
	for (int i = 0; i < selectionState.count(); ++i)
	{
		if (selectionState.at(i))
		{
			buffer += doctypes.at(i).id;
		}
	}
	globalSettings.networkingEngine->docSetFilter(buffer, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void FilterSelectWidget::was_timeout()
{
	title->setText(tr("timeout:") + QString::number(awaiter.getInterval()));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_list_response()
{
	doctypes = RequestParser::interpretAsDocFilterList(awaiter.restext, awaiter.errtext);
	selectionState.clear();
	RequestParser::docFilterResponse::iterator start = doctypes.begin();
	while (start != doctypes.end())
	{
		selectionState.push_back(start->notFiltered);
		++start;
	}
	typesel->reload();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_selection_response()
{
	RequestParser::TypicalResponse resp =  RequestParser::interpretAsSimpliestResponse(awaiter.restext, awaiter.errtext);
	if (resp.resp)
	{
		emit filterApplied();
	}
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}
