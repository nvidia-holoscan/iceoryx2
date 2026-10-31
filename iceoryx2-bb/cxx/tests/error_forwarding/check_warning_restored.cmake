# SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${BUILD_DIR}" --config "${BUILD_CONFIG}"
        --target iceoryx2-error-forwarding-warning-restored
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE errors
    TIMEOUT 20)
if(NOT result MATCHES "^[0-9]+$" OR result EQUAL 0 OR NOT "${output}${errors}" MATCHES "C4702")
    message(FATAL_ERROR "Expected a failed compile specifically diagnosing C4702, got ${result}:\n${output}${errors}")
endif()
