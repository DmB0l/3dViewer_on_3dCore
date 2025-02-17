cmake_minimum_required(VERSION 3.16)

project(3dViewer_on_3dCore LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE cpp ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE h ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE ui ${CMAKE_SOURCE_DIR}/src/*.ui)
file(GLOB_RECURSE qml ${CMAKE_SOURCE_DIR}/src/*.qml)
file(GLOB_RECURSE qrc ${CMAKE_SOURCE_DIR}/*.qrc)

set(PROJECT_SOURCES ${cpp} ${h} ${ui} ${qml} ${qrc})


# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    QuickWidgets
    Qml
    Quick
    Gui
    Network
    3DCore
    3DRender
    3DInput
    3DExtras
    3DAnimation
    3DLogic
)

message("QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Widgets
    QuickWidgets
    Qml
    Quick
    Gui
    Network
    3DCore
    3DRender
    3DInput
    3DExtras
    3DAnimation
    3DLogic
)

find_package(spdlog REQUIRED)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()


target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::3DCore
    Qt${QT_VERSION_MAJOR}::3DRender
    Qt${QT_VERSION_MAJOR}::3DInput
    Qt${QT_VERSION_MAJOR}::3DExtras
    Qt${QT_VERSION_MAJOR}::3DLogic
    Qt${QT_VERSION_MAJOR}::3DAnimation
    spdlog::spdlog
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

