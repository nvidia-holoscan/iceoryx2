// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "iox2/bb/detail/source_location.hpp"
#include "iox2/legacy/error_reporting/custom/error_kind.hpp"
#include "iox2/legacy/error_reporting/violation.hpp"

// Test-only implementation of the supported reporting customization interface.
// In particular panic is deliberately allowed to return, without a noreturn attribute.
namespace iox2 {
namespace legacy {
namespace er {
inline unsigned report_count = 0;

inline void panic(const bb::detail::SourceLocation& location) {
    if (location.line() == 0) {
        std::exit(91);
    }
    std::fputs("panic-returned\n", stderr);
    std::fflush(stderr);
}

template <typename Message>
inline void panic(const bb::detail::SourceLocation& location, Message&& message) {
    if (std::strcmp(message, "message") != 0) {
        std::exit(92);
    }
    std::fputs("panic-message\n", stderr);
    panic(location);
}

template <typename Kind, typename Error>
inline void report(const bb::detail::SourceLocation& location, Kind, const Error& error, const char* condition) {
    if (location.line() == 0 || error.code().value != 42 || error.module().value != 256
        || std::strcmp(condition, "condition") != 0) {
        std::exit(93);
    }
    ++report_count;
    std::fputs("report-condition\n", stderr);
    std::fflush(stderr);
}

template <typename Kind, typename Error, typename Message>
inline void report(const bb::detail::SourceLocation& location,
                   Kind kind,
                   const Error& error,
                   const char* condition,
                   Message&& message) {
    if (std::strcmp(message, "message") != 0) {
        std::exit(94);
    }
    report(location, kind, error, condition);
    std::fputs("report-message\n", stderr);
    std::fflush(stderr);
}
} // namespace er
} // namespace legacy
} // namespace iox2
