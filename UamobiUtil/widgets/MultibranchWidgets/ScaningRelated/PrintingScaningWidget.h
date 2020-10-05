#pragma once
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "NormalScaningWidget.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/ControlsMiniwidgets/ControlManager.h"


class AbsPrinterWrapper;
class SemaphorLabel;
class PrintingScaningWidget : public NormalScaningWidget
{
	Q_OBJECT
protected:

	AbsPrinterWrapper* printerWrapper;
	SemaphorLabel* connectionState;
	void _print(QString text);

	virtual void _handleRecord(RecEntity) override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	virtual void submitPressed() override;
	virtual void backNeeded() override;
	virtual void item_confirmed_response() override;
    virtual void barcodeConfirmed() override;
	virtual void wipe() override;
public:
	explicit PrintingScaningWidget(QWidget* parent = Q_NULLPTR, IndependentBranchNode* searchScr = Q_NULLPTR);
    virtual ~PrintingScaningWidget()
#ifdef QT_VERSION5X
    override
#endif
    ;
protected slots:
	void wrapperOk();
	void wrapperError(QString errtext);
};
