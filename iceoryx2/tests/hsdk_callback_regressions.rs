// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: Apache-2.0 OR MIT

#![cfg(feature = "std")]
use iceoryx2::{
    port::{BackpressureAction, update_connections::UpdateConnections},
    prelude::*,
};
use std::sync::{
    Arc, Mutex,
    atomic::{AtomicUsize, Ordering},
};

#[test]
fn sample_callbacks_preserve_lifetime_overflow_and_backpressure()
-> Result<(), Box<dyn std::error::Error>> {
    // A new private root prevents this test from scanning existing node resources.
    let unique = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)?
        .as_nanos();
    let root = std::env::temp_dir().join(format!("iox10cb-{}-{unique}", std::process::id()));
    assert!(!root.exists());
    std::fs::create_dir(&root)?;
    let root = root.to_str().expect("test root must be UTF-8");
    let mut config = Config::default();
    let root_path = Path::new(root.as_bytes())?;
    config.global.set_root_path(&root_path);
    assert_eq!(config.global.root_path(), &root_path);
    let node = NodeBuilder::new()
        .config(&config)
        .create::<local::Service>()?;
    assert_eq!(node.config().global.root_path(), &root_path);
    let released = Arc::new(Mutex::new(Vec::<u64>::new()));
    let replaced = Arc::new(Mutex::new(Vec::<(u64, u128, u128, u128)>::new()));
    let service = node
        .service_builder(&"callback/lifetime".try_into()?)
        .publish_subscribe::<u64>()
        .subscriber_max_buffer_size(1)
        .enable_safe_overflow(true)
        .create()?;
    let subscriber = service.subscriber_builder().buffer_size(1).create()?;
    let publisher = service
        .publisher_builder()
        .set_sample_released_handler({
            let log = released.clone();
            move |info| {
                assert_eq!(info.payload_len, 8);
                log.lock()
                    .unwrap()
                    .push(unsafe { (info.payload_ptr as *const u64).read_unaligned() });
            }
        })
        .set_sample_replaced_handler({
            let log = replaced.clone();
            move |info| {
                assert_eq!(info.payload_len, 8);
                log.lock().unwrap().push((
                    unsafe { (info.payload_ptr as *const u64).read_unaligned() },
                    info.service_id,
                    info.sender_port_id,
                    info.receiver_port_id,
                ));
            }
        })
        .create()?;
    publisher.update_connections()?;
    subscriber.update_connections()?;
    assert_eq!(publisher.send_copy(41)?, 1);
    let held = subscriber.receive()?.expect("first sample");
    assert_eq!(*held.payload(), 41);
    publisher.send_copy(42)?;
    assert!(
        !released.lock().unwrap().contains(&41),
        "callback before borrower release"
    );
    drop(held);
    publisher.send_copy(43)?;
    let events = replaced.lock().unwrap().clone();
    assert_eq!(events.len(), 1);
    assert_eq!(events[0].0, 42);
    assert_eq!(events[0].1, service.unique_service_id().value());
    assert_eq!(events[0].2, publisher.id().value());
    assert_eq!(events[0].3, subscriber.id().value());
    assert!(released.lock().unwrap().contains(&41));
    assert!(released.lock().unwrap().contains(&42));
    assert_eq!(*subscriber.receive()?.expect("newest sample").payload(), 43);
    drop(publisher);
    drop(subscriber);
    drop(service);
    let calls = Arc::new(AtomicUsize::new(0));
    let service = node
        .service_builder(&"callback/backpressure".try_into()?)
        .publish_subscribe::<u64>()
        .subscriber_max_buffer_size(1)
        .enable_safe_overflow(false)
        .create()?;
    let subscriber = service.subscriber_builder().buffer_size(1).create()?;
    let publisher = service
        .publisher_builder()
        .set_backpressure_handler({
            let calls = calls.clone();
            move |_| {
                calls.fetch_add(1, Ordering::SeqCst);
                BackpressureAction::DiscardDataAndFail
            }
        })
        .create()?;
    publisher.update_connections()?;
    subscriber.update_connections()?;
    assert_eq!(publisher.send_copy(1)?, 1);
    assert_eq!(calls.load(Ordering::SeqCst), 0);
    assert!(publisher.send_copy(2).is_err());
    assert!(calls.load(Ordering::SeqCst) > 0);
    assert_eq!(
        *subscriber
            .receive()?
            .expect("original queued sample")
            .payload(),
        1
    );
    let slices = Arc::new(Mutex::new(Vec::<Vec<u64>>::new()));
    let dynamic = node
        .service_builder(&"callback/dynamic".try_into()?)
        .publish_subscribe::<[u64]>()
        .create()?;
    let publisher = dynamic
        .publisher_builder()
        .initial_max_slice_len(3)
        .set_sample_released_handler({
            let slices = slices.clone();
            move |info| {
                assert_eq!(info.payload_len, 24);
                slices.lock().unwrap().push(
                    unsafe { std::slice::from_raw_parts(info.payload_ptr.cast::<u64>(), 3) }
                        .to_vec(),
                );
            }
        })
        .create()?;
    publisher
        .loan_slice_uninit(3)?
        .write_from_fn(|i| i as u64 + 7)
        .send()?;
    assert_eq!(*slices.lock().unwrap(), vec![vec![7, 8, 9]]);
    println!(
        "PASS dynamic slice length/bytes; fixed payload lifetime, borrowed sample guard, safe-overflow replacement identity, newest delivery, backpressure no-pressure and pressure oracles"
    );
    Ok(())
}
