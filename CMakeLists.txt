cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(SerialPortTool VERSION 1.1 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort Core Gui)

file(GLOB ALL_FILES
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)

qt6_add_resources(RESOURCES_RCC ${CMAKE_SOURCE_DIR}/resources/res.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
            MANUAL_FINALIZATION
            ${CMAKE_SOURCE_DIR}/main.cpp
    )
else()
    add_executable(${PROJECT_NAME}
            ${CMAKE_SOURCE_DIR}/main.cpp
    )
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
    ${ALL_FILES}
    ${RESOURCES_RCC}
)

# 链接 Qt6 库
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SerialPort
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
)

# 控制输出
set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
