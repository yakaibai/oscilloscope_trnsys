cmake_minimum_required(VERSION 3.24.0)
project(Type263)
find_package(FLTK REQUIRED NO_MODULE)
include(${FLTK_USE_FILE})
ADD_LIBRARY(Type263 SHARED Type263.cpp)
target_include_directories(Type263 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Type263 fltk ${PROJECT_SOURCE_DIR}/Libs/TRNDll64.lib)