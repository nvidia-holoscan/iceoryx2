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

use crate::api::{iox2_buffer_16_align_4_t, iox2_callback_context};

/// Read-only pointers into a publisher sample replaced by safe overflow for one receiver.
///
/// Valid only for the duration of the [`iox2_sample_replaced_handler`] call.
#[repr(C)]
pub struct iox2_sample_replaced_info_t {
    pub service_id: iox2_buffer_16_align_4_t,
    pub sender_port_id: iox2_buffer_16_align_4_t,
    pub receiver_port_id: iox2_buffer_16_align_4_t,
    pub user_header_ptr: *const u8,
    pub user_header_len: usize,
    pub payload_ptr: *const u8,
    pub payload_len: usize,
}

/// Callback invoked when safe overflow replaces an unread sample for one receiver.
///
/// # Arguments
///
/// * `info` — const pointer to [`iox2_sample_replaced_info_t`]; valid only during the call
/// * `ctx` — user [`iox2_callback_context`](crate::iox2_callback_context)
///
/// # Safety
///
/// * `ctx` may be stored and invoked from the publisher's internal paths; if the publisher is used
///   from multiple threads, `ctx` must be thread-safe.
pub type iox2_sample_replaced_handler =
    extern "C" fn(*const iox2_sample_replaced_info_t, iox2_callback_context);
