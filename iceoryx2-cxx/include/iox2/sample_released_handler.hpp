// Copyright (c) 2026 Contributors to the Eclipse Foundation
//
// See the NOTICE file(s) distributed with this work for additional
// information regarding copyright ownership.
//
// This program and the accompanying materials are made available under the
// terms of the Apache Software License 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0, or the MIT license
// which is available at https://opensource.org/licenses/MIT.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT

#ifndef IOX2_SAMPLE_RELEASED_HANDLER_HPP
#define IOX2_SAMPLE_RELEASED_HANDLER_HPP

#include "iox2/bb/static_function.hpp"
#include "iox2/internal/callback_context.hpp"
#include "iox2/internal/iceoryx2.hpp"

#include <cstdint>

namespace iox2 {
namespace detail {
inline void sample_released_handler_delegate(const iox2_sample_released_info_t* info_handle,
                                             iox2_callback_context callback_cxt);
} // namespace detail

/// Read-only view of a sample's user header and payload while the publisher pool reclaims the SHM
/// bucket. Pointers are valid only for the duration of the [`SampleReleasedHandler`] call.
class SampleReleasedInfo {
  private:
    const iox2_sample_released_info_t* m_info;

    explicit SampleReleasedInfo(const iox2_sample_released_info_t* info)
        : m_info(info) {
    }

    friend void iox2::detail::sample_released_handler_delegate(const iox2_sample_released_info_t* info_handle,
                                                                 iox2_callback_context callback_cxt);

  public:
    [[nodiscard]] auto user_header_ptr() const -> const uint8_t* {
        return m_info->user_header_ptr;
    }

    [[nodiscard]] auto user_header_len() const -> std::size_t {
        return m_info->user_header_len;
    }

    [[nodiscard]] auto payload_ptr() const -> const uint8_t* {
        return m_info->payload_ptr;
    }

    [[nodiscard]] auto payload_len() const -> std::size_t {
        return m_info->payload_len;
    }
};

/// Invoked when a publisher sample's shared-memory bucket is fully released and is about to be
/// returned to the pool.
using SampleReleasedHandler = iox2::bb::StaticFunction<void(const SampleReleasedInfo&)>;

namespace detail {
inline void sample_released_handler_delegate(const iox2_sample_released_info_t* info_handle,
                                             iox2_callback_context callback_cxt) {
    auto* callback = static_cast<SampleReleasedHandler*>(callback_cxt);
    auto info = SampleReleasedInfo(info_handle);
    (*callback)(info);
}
} // namespace detail

} // namespace iox2

#endif // IOX2_SAMPLE_RELEASED_HANDLER_HPP
