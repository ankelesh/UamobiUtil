# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = UamobiUtil
DESTDIR = ../Win32/Debug
QT += core xml network gui widgets
CONFIG += debug
DEFINES += _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DLL QT_NETWORK_LIB QT_WIDGETS_LIB QT_XML_LIB QT_VERSION5X
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ./debugtrace.h \
    ./networking/global.h \
    ./networking/Parsers/abs_parsed_request.h \
    ./networking/Parsers/RequestParser.h \
    ./networking/Parsers/SimpleLinearParsers.h \
    ./widgets/utils/GlobalAppSettings.h \
    ./widgets/utils/ElementsStyles.h \
    ./networking/things.h \
    ./networking/xml_funcs.h \
    ./UamobiUtil.h \
    ./networking/dataupdateengine.h \
    ./networking/dataupdateengine-http.h \
    ./widgets/utils/SpecializedWidgets.h \
    ./widgets/LoginBranch/LoginSelectWidget.h \
    ./widgets/LoginBranch/LoginWidget.h \
    ./widgets/SettingsBranches/MainSettingsWidget.h \
    ./networking/RequestAwaiter.h \
    ./widgets/MainPageWidget.h \
    ./widgets/utils/EventsAndFilters.h \
    ./widgets/parents/inframedWidget.h \
    ./networking/photo/datarequest.h
SOURCES += ./debugtrace.cpp \
    ./main.cpp \
    ./networking/dataupdateengine-http.cpp \
    ./networking/dataupdateengine.cpp \
    ./networking/Parsers/abs_parsed_request.cpp \
    ./networking/Parsers/RequestParser.cpp \
    ./networking/Parsers/SimpleLinearParsers.cpp \
    ./networking/photo/datarequest.cpp \
    ./networking/RequestAwaiter.cpp \
    ./networking/xml_funcs.cpp \
    ./UamobiUtil.cpp \
    ./widgets/LoginBranch/LoginSelectWidget.cpp \
    ./widgets/LoginBranch/LoginWidget.cpp \
    ./widgets/MainPageWidget.cpp \
    ./widgets/SettingsBranches/MainSettingsWidget.cpp \
    ./widgets/utils/ElementsStyles.cpp \
    ./widgets/utils/SpecializedWidgets.cpp
RESOURCES += UamobiUtil.qrc
TRANSLATIONS += translations/uamobi_en.ts translations/uamobi_ro.ts translations/uamobi_ru.ts
