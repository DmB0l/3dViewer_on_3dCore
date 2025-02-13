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

find_package(Qt5 REQUIRED COMPONENTS
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

add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::QuickWidgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Gui
    Qt5::Network
    Qt5::3DCore
    Qt5::3DRender
    Qt5::3DInput
    Qt5::3DExtras
    Qt5::3DLogic
    Qt5::3DAnimation
    spdlog::spdlog
)

