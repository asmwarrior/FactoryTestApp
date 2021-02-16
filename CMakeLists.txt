cmake_minimum_required(VERSION 3.5)

project(FactoryTestApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Qml Sql Network SerialPort REQUIRED)

set(SOURCES
    main.cpp
    AppComponent.cpp
    MainWindow.cpp
    Session.h
    Database.cpp
    TestSequence.cpp
    JLinkManager.cpp
    Logger.cpp
    RailtestClient.cpp
    TestFixtureWidget.cpp
    DutButton.cpp
    SessionInfoWidget.cpp
    DutInfoWidget.cpp
    ActionHintWidget.cpp
)


add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${PROJECT_NAME}.qrc
    ${PROJECT_NAME}.rc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Widgets
        Qt5::Qml
        Qt5::Sql
        Qt5::Network
        Qt5::SerialPort
)
