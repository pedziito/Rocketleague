cmake_minimum_required(VERSION 3.20)
project(ControllerEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(WIN32)
    add_library(ControllerEngine SHARED
            ../src/cpp/HIDController/HIDController.cpp
            ../src/cpp/HIDController/InputProcessor.cpp
            ../src/cpp/HIDController/AntiDrift.cpp
            ../src/cpp/PollingRateOptimizer/PollingRateOptimizer.cpp
            ../src/cpp/PerformanceMonitor/PerformanceMonitor.cpp
            ../src/cpp/HIDController/ControllerEngine.cpp
    )

    target_include_directories(ControllerEngine PRIVATE
            ${CMAKE_SOURCE_DIR}/../src/cpp
            ${CMAKE_SOURCE_DIR}/../src/shared
    )

    target_compile_definitions(ControllerEngine PRIVATE WIN32_LEAN_AND_MEAN)
    target_link_libraries(ControllerEngine PRIVATE setupapi winusb)

    if (MSVC)
        target_compile_options(ControllerEngine PRIVATE /W4)
    else()
        target_compile_options(ControllerEngine PRIVATE -Wall -Wextra)
    endif()

    set_target_properties(ControllerEngine PROPERTIES
            OUTPUT_NAME "ControllerEngine"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    )
else()
    message(STATUS "Non-Windows platform detected: skipping ControllerEngine native build")
    add_custom_target(ControllerEngine_stub ALL
        COMMENT "ControllerEngine native build skipped on non-Windows platforms"
    )
endif()
