cmake_minimum_required(VERSION 3.10.0)
project(Chess960 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Svg Network)

include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

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
    src/network/networkclient.cpp
    src/network/networkmanager.cpp
    src/network/networkserver.cpp
    ${RESOURCES}
)

add_executable(Chess960Tests
    tests/test_main.cpp
    tests/test_board.cpp
    src/board.cpp
    src/piece.cpp
    src/move.cpp
    src/movehistory.cpp
)

target_link_libraries(Chess960Tests PRIVATE
    gtest_main
    Qt6::Core
)

target_compile_definitions(Chess960Tests PRIVATE
    QT_CORE_LIB
    QT_NO_DEBUG
)

target_include_directories(Chess960Tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(Chess960 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Network
)

target_include_directories(Chess960 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

enable_testing()
add_test(NAME Chess960Tests COMMAND Chess960Tests)