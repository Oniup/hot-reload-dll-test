cmake_minimum_required(VERSION 3.2)

project(hot-reload-test)



add_library(
    dll_test SHARED

    ${CMAKE_CURRENT_SOURCE_DIR}/dll_test.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dll_test.cpp
)

target_compile_definitions(
    dll_test

    PUBLIC API_BUILD_DLL
)

target_include_directories(
    dll_test

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET dll_test PROPERTY CXX_STANDARD 20)




add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(
    ${CMAKE_PROJECT_NAME}

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

if (WIN32)
    target_compile_definitions(
        ${CMAKE_PROJECT_NAME}

        PUBLIC _CRT_SECURE_NO_WARNINGS
    )
endif ()

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)