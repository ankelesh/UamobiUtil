# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ./UamobiUtil.h \
    ./networking/QueryTemplates.h \
    ./networking/dataupdateengine-http.h \
    ./networking/RequestAwaiter.h \
    ./networking/things.h \
    ./datacore/UniformXmlObject.h \
    ./networking/Parsers/abs_parsed_request.h \
    ./networking/Parsers/DynamicLinearParsers.h \
    ./networking/Parsers/RequestParser.h \
    ./networking/Parsers/SimpleLinearParsers.h \
    ./networking/Parsers/SimpleResponseParsers.h \
    ./widgets/LoginBranch/LoginWidget.h \
    ./widgets/SettingsBranches/MainSettingsWidget.h \
    ./widgets/MainPageWidget.h \
    ./debugtrace.h \
    ./widgets/utils/GlobalAppSettings.h \
    ./widgets/utils/ElementsStyles.h \
    ./widgets/utils/EventsAndFilters.h \
    ./legacy/3drparty/tty/tty.h \
    ./legacy/legacy.h \
    ./legacy/qtCompatibility/scrollgrabber.h \
    ./widgets/ElementWidgets/BigButtonsSpinbox.h \
    ./widgets/ElementWidgets/MegaIconButton.h \
    ./widgets/ElementWidgets/ProcessingOverlay.h \
    ./widgets/ControlsMiniwidgets/ControlManager.h \
    ./widgets/ControlsMiniwidgets/QuantityControl.h \
    ./widgets/ControlsMiniwidgets/abs_control.h \
    ./widgets/ControlsMiniwidgets/ControlTranslator.h \
    ./ScaningCore/BarcodeObserver.h \
    ./widgets/MultibranchWidgets/ControlListWidget.h \
    ./widgets/MultibranchWidgets/DocResultsWidget.h \
    ./widgets/MultibranchWidgets/FilterSelectWidget.h \
    ./widgets/MultibranchWidgets/IdDependentSelectWidget.h \
    ./widgets/MultibranchWidgets/InventoryParamsWidget.h \
    ./widgets/MultibranchWidgets/NormalScaningWidget.h \
    ./widgets/MultibranchWidgets/PagedSearchWidget.h \
    ./widgets/MultibranchWidgets/ParentDocumentWidget.h \
    ./widgets/MultibranchWidgets/PlaceSelectionWidget.h \
    ./widgets/MultibranchWidgets/ReceiptParametersWidget.h \
    ./widgets/MultibranchWidgets/SelectItemFromListWidget.h \
    ./widgets/MultibranchWidgets/PrintingScaningWidget.h \
    ./widgets/ExtendedDelegates/CheckableDelegate.h \
    ./widgets/ExtendedDelegates/ZebraListItemDelegate.h \
    ./widgets/BranchingTools/BranchDescriptionParser.h \
    ./widgets/BranchingTools/BranchElementDescription.h \
    ./widgets/BranchingTools/BranchFactory.h \
    ./widgets/BranchingTools/AbsBranch.h \
    ./widgets/BranchingTools/AdjustableBranch.h \
    ./widgets/BranchingTools/StaticBranch.h \
    ./widgets/Branches/SenderNode.h \
    ./widgets/Branches/SwitchSubbranch.h \
    ./widgets/BranchingTools/EmbeddedBranches.h \
    ./widgets/ModeSelectionBranch/ModeSelectionWidget.h \
    ./widgets/MultibranchWidgets/BarcodeFilterSelectionSubbranch.h \
    ./datacore/EntitledEntity.h \
    ./datacore/FullDocumentEntity.h \
    ./datacore/FullItemEntity.h \
    ./datacore/InputControlEntity.h \
    ./datacore/ModeEntity.h \
    ./datacore/NamedIdEntity.h \
    ./datacore/AbsEntityPrototype.h \
    ./widgets/parents/abstractNodeInterface.h \
    ./widgets/parents/AbstractScaningWidget.h \
    ./widgets/parents/IndependentBranchNode.h \
    ./widgets/parents/inframedWidget.h
SOURCES += ./main.cpp \
    ./UamobiUtil.cpp \
    ./networking/dataupdateengine-http.cpp \
    ./networking/QueryTemplates.cpp \
    ./networking/RequestAwaiter.cpp \
    ./networking/things.cpp \
    ./networking/Parsers/abs_parsed_request.cpp \
    ./networking/Parsers/DynamicLinearParsers.cpp \
    ./networking/Parsers/RequestParser.cpp \
    ./networking/Parsers/SimpleLinearParsers.cpp \
    ./networking/Parsers/SimpleResponseParsers.cpp \
    ./widgets/LoginBranch/LoginWidget.cpp \
    ./widgets/MainPageWidget.cpp \
    ./widgets/SettingsBranches/MainSettingsWidget.cpp \
    ./debugtrace.cpp \
    ./widgets/utils/ElementsStyles.cpp \
    ./widgets/utils/EventsAndFilters.cpp \
    ./widgets/utils/GlobalAppSettings.cpp \
    ./widgets/ModeSelectionBranch/ModeSelectionWidget.cpp \
    ./legacy/3drparty/tty/tty.cpp \
    ./legacy/legacy.cpp \
    ./legacy/qtCompatibility/scrollgrabber.cpp \
    ./widgets/ElementWidgets/BigButtonsSpinbox.cpp \
    ./widgets/ElementWidgets/MegaIconButton.cpp \
    ./widgets/ElementWidgets/ProcessingOverlay.cpp \
    ./widgets/ControlsMiniwidgets/abs_control.cpp \
    ./widgets/ControlsMiniwidgets/ControlManager.cpp \
    ./widgets/ControlsMiniwidgets/ControlTranslator.cpp \
    ./widgets/ControlsMiniwidgets/QuantityControl.cpp \
    ./ScaningCore/BarcodeObserver.cpp \
    ./widgets/MultibranchWidgets/BarcodeFilterSelectionSubbranch.cpp \
    ./widgets/MultibranchWidgets/ControlListWidget.cpp \
    ./widgets/MultibranchWidgets/DocResultsWidget.cpp \
    ./widgets/MultibranchWidgets/FilterSelectWidget.cpp \
    ./widgets/MultibranchWidgets/IdDependentSelectWidget.cpp \
    ./widgets/MultibranchWidgets/InventoryParamsWidget.cpp \
    ./widgets/MultibranchWidgets/NormalScaningWidget.cpp \
    ./widgets/MultibranchWidgets/PagedSearchWidget.cpp \
    ./widgets/MultibranchWidgets/ParentDocumentWidget.cpp \
    ./widgets/MultibranchWidgets/PlaceSelectionWidget.cpp \
    ./widgets/MultibranchWidgets/PrintingScaningWidget.cpp \
    ./widgets/MultibranchWidgets/ReceiptParametersWidget.cpp \
    ./widgets/MultibranchWidgets/SelectItemFromListWidget.cpp \
    ./widgets/BranchingTools/AbsBranch.cpp \
    ./widgets/BranchingTools/AdjustableBranch.cpp \
    ./widgets/BranchingTools/BranchDescriptionParser.cpp \
    ./widgets/BranchingTools/BranchElementDescription.cpp \
    ./widgets/BranchingTools/BranchFactory.cpp \
    ./widgets/BranchingTools/StaticBranch.cpp \
    ./widgets/Branches/SenderNode.cpp \
    ./widgets/Branches/SwitchSubbranch.cpp \
    ./widgets/BranchingTools/EmbeddedBranches.cpp \
    ./widgets/ExtendedDelegates/CheckableDelegate.cpp \
    ./widgets/ExtendedDelegates/ZebraListItemDelegate.cpp \
    ./datacore/AbsEntityPrototype.cpp \
    ./datacore/EntitledEntity.cpp \
    ./datacore/FullDocumentEntity.cpp \
    ./datacore/FullItemEntity.cpp \
    ./datacore/InputControlEntity.cpp \
    ./datacore/ModeEntity.cpp \
    ./datacore/NamedIdEntity.cpp \
    ./datacore/UniformXmlObject.cpp \
    ./widgets/parents/abstractNodeInterface.cpp \
    ./widgets/parents/AbstractScaningWidget.cpp \
    ./widgets/parents/IndependentBranchNode.cpp \
    ./widgets/parents/inframedWidget.cpp
RESOURCES += UamobiUtil.qrc