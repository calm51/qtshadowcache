cmake_minimum_required(VERSION 3.16)

project(qtshadowcache LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(qtshadowcache SHARED
  src/qtshadowcache_global.h
  src/qtshadowcache.cpp
  src/qtshadowcache.h
)

target_link_libraries(qtshadowcache PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(qtshadowcache PRIVATE QTSHADOWCACHE_LIBRARY)


include("./output/makeme.cmake")


