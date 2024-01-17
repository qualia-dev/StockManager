cmake_minimum_required(VERSION 3.5)

project(StockMan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StockMan
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        tradingday.cpp
        tradingday.h
        tradinghistory.cpp
        tradinghistory.h
        formtabsettings.cpp
        formtabsettings.h
        formtabsettings.ui
        date.h
        date.cpp
        company.h company.cpp
        formtabdownload.h formtabdownload.cpp formtabdownload.ui
        ftpdownloader.h
        marketnasdaq.h marketnasdaq.cpp
        stock.h stock.cpp
        marketplace.h marketplace.cpp
        country.h country.cpp
        formtabdatabase.h formtabdatabase.cpp formtabdatabase.ui
        formtabstocks.h formtabstocks.cpp formtabstocks.ui
        dlgexecutesql.h dlgexecutesql.cpp dlgexecutesql.ui
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET StockMan APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(StockMan SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(StockMan
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# add shared library named libQtDb.so
add_library(SqliteWrap SHARED IMPORTED)
target_link_libraries(StockMan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network SqliteWrap)

# Set target properties : path to the pre-built library file
set_target_properties(SqliteWrap PROPERTIES IMPORTED_LOCATION ../../build-SqliteWrap-Desktop_Qt_6_6_1_GCC_64bit-Debug/libSqliteWrap.so)
# Set include directories if needed
target_include_directories(SqliteWrap INTERFACE ../../SqliteWrap)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.StockMan)
endif()
set_target_properties(StockMan PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS StockMan
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StockMan)
endif()
