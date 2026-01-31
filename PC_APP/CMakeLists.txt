cmake_minimum_required(VERSION 3.16)
project(PC_APP)

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network SerialPort)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(PC_APP main.cpp
        Inc/DeviceClient.h
        Src/DeviceClient.cpp
        Inc/AngleDial.h
        Src/AngleDial.cpp
        Inc/SerialConsole.h
        Inc/MainWindow.h
        Src/MainWindow.cpp)

target_link_libraries(PC_APP
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::SerialPort
)