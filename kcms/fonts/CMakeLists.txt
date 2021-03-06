# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_fonts\")

########### next target ###############

set(kcm_fonts_PART_SRCS
    previewrenderengine.cpp
    previewimageprovider.cpp
    fonts.cpp
    fontsaasettings.cpp
    kxftconfig.cpp
    ../kfontinst/lib/FcEngine.cpp
    ../kcms-common_p.h
)

kcmutils_generate_module_data(
    kcm_fonts_PART_SRCS
    MODULE_DATA_HEADER fontsdata.h
    MODULE_DATA_CLASS_NAME FontsData
    SETTINGS_HEADERS fontssettings.h fontsaasettings.h
    SETTINGS_CLASSES FontsSettings FontsAASettings
)

kconfig_add_kcfg_files(kcm_fonts_PART_SRCS fontssettings.kcfgc fontsaasettingsbase.kcfgc GENERATE_MOC)

add_library(kcm_fonts MODULE ${kcm_fonts_PART_SRCS})

target_link_libraries(kcm_fonts KF5::I18n KF5::WindowSystem KF5::KCMUtils KF5::QuickAddons KF5::Declarative kfontinst krdb)

if(X11_FOUND)
    target_link_libraries(kcm_fonts Qt::X11Extras X11::X11 X11::Xft XCB::IMAGE)
endif()

kcoreaddons_desktop_to_json(kcm_fonts "kcm_fonts.desktop")

########### install files ###############
install(FILES fontssettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
set_target_properties(kcm_fonts PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kcms")
install(TARGETS kcm_fonts DESTINATION ${KDE_INSTALL_PLUGINDIR}/kcms)
install(FILES kcm_fonts.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
kpackage_install_package(package kcm_fonts kcms)
