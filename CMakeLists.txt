cmake_minimum_required(VERSION 3.5)

project(CapelonTestStation LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JLinkSDK REQUIRED)
find_package(Qt5 COMPONENTS Widgets Qml Sql Network SerialPort REQUIRED)

set(HEADERS
    ActionHintWidget.h
    Database.h
    Dut.h
    DutButton.h
    DutInfoWidget.h
    JLinkManager.h
    Logger.h
    MainWindow.h
    portmanager.h
    PrinterManager.h
    RailtestClient.h
    SessionInfoWidget.h
    SessionManager.h
    SlipProtocol.h
    TestClient.h
    TestFixtureWidget.h
    TestMethodManager.h
)

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

include_directories(
    ${JLINKSDK_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} WIN32
    ${HEADERS}
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
        ${JLINKSDK_LIBRARY}
        ws2_32
)
