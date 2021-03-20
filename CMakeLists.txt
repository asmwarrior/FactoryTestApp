cmake_minimum_required(VERSION 3.5)

project(CapelonTestStation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Qml Sql Network SerialPort REQUIRED)

set(SOURCES
    main.cpp
    MainWindow.cpp
    SessionManager.cpp
    Dut.h
    Database.cpp
    TestMethodManager.cpp
    JLinkManager.cpp
    Logger.cpp
    RailtestClient.cpp
    PortManager.cpp
    TestClient.cpp
    TestFixtureWidget.cpp
    DutButton.cpp
    SessionInfoWidget.cpp
    DutInfoWidget.cpp
    ActionHintWidget.cpp
    PrinterManager.cpp
)


add_executable(${PROJECT_NAME}
    ${SOURCES}
    "FactoryTestApp.qrc"
    "FactoryTestApp.rc"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Widgets
        Qt5::Qml
        Qt5::Sql
        Qt5::Network
        Qt5::SerialPort
        "${CMAKE_SOURCE_DIR}/JLinkSDK/lib/JLinkARM.lib"
)
