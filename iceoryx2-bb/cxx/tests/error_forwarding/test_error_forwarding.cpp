// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <csignal>
#include <cstdio>
#include <cstdlib>

#include <gtest/gtest.h>

#include "iox2/legacy/error_reporting/error_forwarding.hpp"
#include "iox2/legacy/error_reporting/violation.hpp"

namespace {
using namespace iox2::legacy::er;
using iox2::bb::detail::SourceLocation;

const Violation kError { ErrorCode { 42 }, ModuleId { 256 } };

void forward_panic() {
    forwardPanic(SourceLocation::current(), "message");
}

void forward_fatal() {
    forwardFatalError(kError, ENFORCE_VIOLATION, SourceLocation::current(), "condition");
}

void forward_fatal_message() {
    forwardFatalError(kError, ENFORCE_VIOLATION, SourceLocation::current(), "condition", "message");
}

void run_forwarder(void (*forwarder)()) {
#if defined(_WIN32)
    // Keep CRT dialogs and Windows Error Reporting out of these bounded death-test children.
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
    // A normal-returning indirect call keeps the sentinel independent of noreturn optimization.
    void (*volatile invoke)() = forwarder;
    invoke();
    std::fputs("post-forwarder\n", stderr);
    std::fflush(stderr);
    std::exit(0);
}

#if defined(_WIN32)
const auto abort_status = ::testing::ExitedWithCode(3);
#else
const auto abort_status = ::testing::KilledBySignal(SIGABRT);
#endif

#if defined(IOX2_TEST_RETURNING_PANIC)
constexpr const char* PANIC_OUTPUT = "panic-message\r?\npanic-returned";
constexpr const char* FATAL_OUTPUT = "report-condition\r?\npanic-returned";
constexpr const char* FATAL_MESSAGE_OUTPUT = "report-condition\r?\nreport-message\r?\npanic-returned";
#else
constexpr const char* PANIC_OUTPUT = "\\[PANIC\\] message";
constexpr const char* FATAL_OUTPUT = "\\[PANIC\\]";
constexpr const char* FATAL_MESSAGE_OUTPUT = "\\[PANIC\\]";
#endif

TEST(ErrorForwardingDeathTest, PanicAlwaysAborts) {
    EXPECT_EXIT(run_forwarder(forward_panic), abort_status, PANIC_OUTPUT);
}

TEST(ErrorForwardingDeathTest, FatalErrorAlwaysAborts) {
    EXPECT_EXIT(run_forwarder(forward_fatal), abort_status, FATAL_OUTPUT);
}

TEST(ErrorForwardingDeathTest, FatalErrorWithMessageAlwaysAborts) {
    EXPECT_EXIT(run_forwarder(forward_fatal_message), abort_status, FATAL_MESSAGE_OUTPUT);
}

TEST(ErrorForwarding, NonFatalReportReturns) {
#if defined(IOX2_TEST_RETURNING_PANIC)
    const auto before = report_count;
#endif
    forwardNonFatalError(kError, RUNTIME_ERROR, SourceLocation::current(), "condition");
#if defined(IOX2_TEST_RETURNING_PANIC)
    EXPECT_EQ(report_count, before + 1);
#endif
    SUCCEED();
}
} // namespace

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
