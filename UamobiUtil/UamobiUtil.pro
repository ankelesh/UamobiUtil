# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = UamobiUtil
QT += core xml network gui widgets
CONFIG += debug
DEFINES += DEBUG LINELLA
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += QT_VERSION5X
lessThan(QT_MAJOR_VERSION, 5) : DEFINES += "Q_NULLPTR=0"
lessThan(QT_MAJOR_VERSION, 5) : QT += phonon
android: QT += bluetooth
win32: DEFINES += "Q_FALLTHROUGH=__noop"
win32: RC_FILE += uamobiutil.rc
win32: LIBS += "C:\Program Files\Microsoft SDKs\Windows\v7.0\Lib\WinMM.lib"

wince* {
        INCLUDEPATH += C:/Qt/4.8.3-CE6-static/include
        CONFIG -= debug
        CONFIG += release
        QT -= widgets
        DEFINES += FTR_COM
        QMAKE_CXX += -Ox -GL
        QMAKE_LFLAGS += /LTCG
        RC_FILE = uamobiutilwince.rc
        DEFINES += "Q_FALLTHROUGH=__noop"
        LIBS += "C:\Program Files\Windows CE Tools\wce600\CE600\Lib\ARMV4I\coredll.lib"

}
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
    ./ScaningCore/BarcodeObserver.h \
    ./widgets/MultibranchWidgets/InventoryParamsWidget.h \
    ./widgets/MultibranchWidgets/PlaceSelectionWidget.h \
    ./widgets/MultibranchWidgets/ReceiptParametersWidget.h \
    ./widgets/MultibranchWidgets/DocResultsWidget.h \
    ./widgets/MultibranchWidgets/FilterSelectWidget.h \
    ./widgets/MultibranchWidgets/IdDependentSelectWidget.h \
    ./widgets/MultibranchWidgets/PagedSearchWidget.h \
    ./widgets/MultibranchWidgets/ParentDocumentWidget.h \
    ./widgets/MultibranchWidgets/SelectItemFromListWidget.h \
    ./widgets/MultibranchWidgets/ScaningRelated/ControlListWidget.h \
    ./widgets/MultibranchWidgets/ScaningRelated/MulticontrolScaningWidget.h \
    ./widgets/MultibranchWidgets/ScaningRelated/NormalScaningWidget.h \
    ./widgets/MultibranchWidgets/ScaningRelated/ObservedScaningWidget.h \
    ./widgets/MultibranchWidgets/ScaningRelated/PrintingScaningWidget.h \
    ./widgets/MultibranchWidgets/Observers/ListPickObserver.h \
    ./widgets/ExtendedDelegates/CheckableDelegate.h \
    ./widgets/ExtendedDelegates/ZebraListItemDelegate.h \
    ./widgets/BranchingTools/BranchDescriptionParser.h \
    ./widgets/BranchingTools/BranchElementDescription.h \
    ./widgets/BranchingTools/BranchFactory.h \
    ./widgets/BranchingTools/AbsBranch.h \
    ./widgets/BranchingTools/AdjustableBranch.h \
    ./widgets/BranchingTools/StaticBranch.h \
    ./widgets/MultibranchWidgets/Observers/SkippedNode.h \
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
    ./widgets/parents/inframedWidget.h \
    ./widgets/ControlsMiniwidgets/ControlManager.h \
    ./widgets/ControlsMiniwidgets/QuantityControl.h \
    ./widgets/ControlsMiniwidgets/abs_control.h \
    ./widgets/ControlsMiniwidgets/BarcodeControl.h \
    ./widgets/ControlsMiniwidgets/ControlTranslator.h \
    ./widgets/ControlsMiniwidgets/DateTimeControl.h \
    ./widgets/ControlsMiniwidgets/LabelControl.h \
    ./widgets/ControlsMiniwidgets/StringControl.h \
    PrinterWrappers/AbsPrinterWrapper.h \
    PrinterWrappers/AndroidBluetoothPrinterWrapper.h \
    PrinterWrappers/FTRCOMPrinterWrapper.h \
    PrinterWrappers/NullPrinterWrapper.h \
    PrinterWrappers/PrinterWrapperFactory.h \
    widgets/BranchingTools/BranchException.h \
    widgets/ElementWidgets/ExtendedDialogs.h \
    widgets/ElementWidgets/ExtendedLabels.h \
    widgets/MultibranchWidgets/Selectors/EditableDocResultsWidget.h \
    widgets/ExtendedDelegates/CountingDelegate.h \
    datacore/PseudotableEntityModel.h \
    widgets/MultibranchWidgets/FlowControls/SwitchByScannedCodeNode.h \
    widgets/utils/client_defaults.h \
    SoundWrappers/AbsSoundWrapper.h \
    SoundWrappers/PhononSoundWrapper.h \
    SoundWrappers/ModernQtSoundWrapper.h \
    SoundWrappers/SoundEffectPlayer.h \
    SoundWrappers/WinCEUnpackWrapper.h
SOURCES += ./main.cpp \
    ./UamobiUtil.cpp \
    ./widgets/ControlsMiniwidgets/BarcodeControl.cpp \
    ./widgets/ControlsMiniwidgets/DateTimeControl.cpp \
    ./widgets/ControlsMiniwidgets/LabelControl.cpp \
    ./widgets/ControlsMiniwidgets/StringControl.cpp \
    ./widgets/MultibranchWidgets/Observers/ListPickObserver.cpp \
    ./widgets/MultibranchWidgets/Observers/SkippedNode.cpp \
    ./widgets/MultibranchWidgets/ScaningRelated/ObservedScaningWidget.cpp \
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
    ./widgets/MultibranchWidgets/DocResultsWidget.cpp \
    ./widgets/MultibranchWidgets/FilterSelectWidget.cpp \
    ./widgets/MultibranchWidgets/IdDependentSelectWidget.cpp \
    ./widgets/MultibranchWidgets/InventoryParamsWidget.cpp \
    ./widgets/MultibranchWidgets/PagedSearchWidget.cpp \
    ./widgets/MultibranchWidgets/ParentDocumentWidget.cpp \
    ./widgets/MultibranchWidgets/PlaceSelectionWidget.cpp \
    ./widgets/MultibranchWidgets/ReceiptParametersWidget.cpp \
    ./widgets/MultibranchWidgets/SelectItemFromListWidget.cpp \
    ./widgets/MultibranchWidgets/ScaningRelated/ControlListWidget.cpp \
    ./widgets/MultibranchWidgets/ScaningRelated/MulticontrolScaningWidget.cpp \
    ./widgets/MultibranchWidgets/ScaningRelated/NormalScaningWidget.cpp \
    ./widgets/MultibranchWidgets/ScaningRelated/PrintingScaningWidget.cpp \
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
    ./widgets/parents/inframedWidget.cpp \
    PrinterWrappers/AbsPrinterWrapper.cpp \
    PrinterWrappers/AndroidBluetoothPrinterWrapper.cpp \
    PrinterWrappers/FTRCOMPrinterWrapper.cpp \
    PrinterWrappers/NullPrinterWrapper.cpp \
    PrinterWrappers/PrinterWrapperFactory.cpp \
    widgets/BranchingTools/BranchException.cpp \
    widgets/ElementWidgets/ExtendedDialogs.cpp \
    widgets/ElementWidgets/ExtendedLabels.cpp \
    widgets/MultibranchWidgets/Selectors/EditableDocResultsWidget.cpp \
    widgets/ExtendedDelegates/CountingDelegate.cpp \
    datacore/PseudotableEntityModel.cpp \
    widgets/MultibranchWidgets/FlowControls/SwitchByScannedCodeNode.cpp \
    SoundWrappers/AbsSoundWrapper.cpp \
    SoundWrappers/ModernQtSoundWrapper.cpp \
    SoundWrappers/PhononSoundWrapper.cpp \
    SoundWrappers/SoundEffectPlayer.cpp \
    SoundWrappers/WinCEUnpackWrapper.cpp
RESOURCES += UamobiUtil.qrc
TRANSLATIONS += translations/uamobiutil_ru.ts translations/uamobiutil_ro.ts translations/uamobiutil_en.ts

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
