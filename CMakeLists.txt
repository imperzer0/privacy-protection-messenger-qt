cmake_minimum_required(VERSION 3.5)

project(privacy-protection-messenger-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE generator - ${CMAKE_GENERATOR}")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TRANSLATIONS privacy-protection-messenger-qt_uk_UA)
set(TS_FILES)

foreach (TS_FILE IN ITEMS ${TRANSLATIONS})
    set(TS_FILES ${TS_FILES} "${TS_FILE}.ts")
endforeach ()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        palette.xml
        ${TS_FILES}
        )

set(PROJECT_RESOURCES
        resources.qrc
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(privacy-protection-messenger-qt MANUAL_FINALIZATION ${PROJECT_SOURCES} ${PROJECT_RESOURCES})

    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET privacy-protection-messenger-qt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    message("TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES OPTIONS -compress")
    qt_add_lrelease(privacy-protection-messenger-qt TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES OPTIONS -compress)

    foreach (TSFILE IN ITEMS ${TRANSLATIONS})
        add_custom_command(TARGET privacy-protection-messenger-qt_lrelease
                COMMAND cp "${TSFILE}.qm" "${CMAKE_SOURCE_DIR}/${TSFILE}.qm")
    endforeach ()

else ()
    if (ANDROID)
        add_library(privacy-protection-messenger-qt SHARED ${PROJECT_SOURCES} ${PROJECT_RESOURCES})

        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(privacy-protection-messenger-qt ${PROJECT_SOURCES} ${PROJECT_RESOURCES})
    endif ()

    #    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
    message("TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES OPTIONS -compress")
    qt5_add_translation(privacy-protection-messenger-qt_translations ${TS_FILES})

    foreach (TSFILE IN ITEMS ${TRANSLATIONS})
        add_custom_command(TARGET privacy-protection-messenger-qt
                COMMAND cp "${TSFILE}.qm" "${CMAKE_SOURCE_DIR}/${TSFILE}.qm")
    endforeach ()

endif ()

target_link_libraries(privacy-protection-messenger-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(privacy-protection-messenger-qt PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(privacy-protection-messenger-qt)
endif ()
