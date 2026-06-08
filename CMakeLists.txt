cmake_minimum_required(VERSION 3.10.0)
project(Chess960 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Svg)

# Создаём ресурсы QML - ПРАВИЛЬНЫЙ ПУТЬ
qt6_add_resources(RESOURCES qml/qml.qrc)

add_executable(Chess960
    src/main.cpp
    src/gamewindow.cpp
    src/gamecontroller.cpp
    src/board.cpp
    src/piece.cpp
    src/move.cpp
    src/movehistory.cpp
    src/aiplayer.cpp
    ${RESOURCES}
)

target_link_libraries(Chess960 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
)

target_include_directories(Chess960 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)