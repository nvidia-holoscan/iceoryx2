# SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.22)
project(iceoryx2-error-forwarding-tests LANGUAGES CXX)

if(NOT TARGET iceoryx2-bb-cxx::iceoryx2-bb-cxx)
    find_package(iceoryx2-bb-cxx CONFIG REQUIRED)
endif()
if(NOT TARGET GTest::gtest)
    find_package(iceoryx2-cmake-modules CONFIG REQUIRED)
    include(Iceoryx2GTest)
endif()

enable_testing()
foreach(reporting IN ITEMS default returning)
    set(test_target iceoryx2-error-forwarding-${reporting})
    add_executable(${test_target} test_error_forwarding.cpp)
    target_link_libraries(${test_target} PRIVATE iceoryx2-bb-cxx::iceoryx2-bb-cxx GTest::gtest)
    target_compile_features(${test_target} PRIVATE cxx_std_17)
    if(reporting STREQUAL "returning")
        # Override only the documented source reporting customization seam.
        target_include_directories(${test_target} BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/returning")
        target_compile_definitions(${test_target} PRIVATE IOX2_TEST_RETURNING_PANIC)
    endif()
    if(MSVC)
        target_compile_options(${test_target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${test_target} PRIVATE -Wall -Wextra -Werror)
    endif()
    add_test(NAME error-forwarding-${reporting} COMMAND ${test_target})
    set_tests_properties(error-forwarding-${reporting} PROPERTIES TIMEOUT 20)
endforeach()

if(MSVC)
    # This translation unit must still diagnose C4702 after including the real header.
    add_executable(iceoryx2-error-forwarding-warning-restored EXCLUDE_FROM_ALL test_warning_restored.cpp)
    target_link_libraries(iceoryx2-error-forwarding-warning-restored PRIVATE iceoryx2-bb-cxx::iceoryx2-bb-cxx)
    target_compile_options(iceoryx2-error-forwarding-warning-restored PRIVATE /W4 /WX /Od)
    add_test(NAME error-forwarding-warning-restored
        COMMAND "${CMAKE_COMMAND}"
            "-DBUILD_DIR=${CMAKE_BINARY_DIR}"
            "-DBUILD_CONFIG=$<CONFIG>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/check_warning_restored.cmake")
    set_tests_properties(error-forwarding-warning-restored PROPERTIES TIMEOUT 30 RUN_SERIAL TRUE)
endif()
