<!--
SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
SPDX-License-Identifier: Apache-2.0
-->

# iceoryx2 for NVIDIA Holoscan SDK

This public fork carries iceoryx2 changes used by
[NVIDIA Holoscan SDK](https://github.com/nvidia-holoscan/holoscan-sdk) and its
integrations. The upstream project is
[Eclipse iceoryx2](https://github.com/eclipse-iceoryx/iceoryx2).

## Branches and source snapshots

| Reference | Purpose |
| --- | --- |
| `nvidia` | Fork documentation and administration, with independent history |
| `main` | Upstream iceoryx2 content |
| `holoscan-sdk/vX.Y` | Public SDK development and maintenance for one release line |
| `holoscan-sdk-<sdk-tag>` | Immutable snapshot for a validated public SDK release |
| `holoscan-sdk/checkpoints/<upstream-ref>-<base-sha>/rN` | Development checkpoints for iceoryx2 patch series |
| Maintainer topic branches and pull requests | Review of fork maintenance changes |

Fork maintainers develop directly on GitHub. Maintenance branches may advance
before an SDK release is qualified; their names do not assert release readiness. Consumer
build definitions pin a full commit SHA. Release tags identify the exact source
selected for a validated public SDK release and are never moved or reused.
Public preview tags retain the SDK's exact preview-tag spelling.

The version in `holoscan-sdk/vX.Y` is the SDK version. Under
`holoscan-sdk/checkpoints/`, the version is the iceoryx2 upstream ref.
For example, `holoscan-sdk/checkpoints/v0.10.0-135d09dd8b29/r1` names the
first revision of a series based on iceoryx2 `v0.10.0`. The 12-character
SHA identifies the upstream base commit.

> [!WARNING]
> `rN` checkpoint branches may be deleted or force-pushed at any time.
> A pinned commit can become unavailable if it is no longer reachable from a
> branch or tag on the public fork (for example, `holoscan-sdk/vX.Y` or
> `holoscan-sdk-<sdk-tag>`).

## Contributions

This fork does not accept external contributions. Submit improvements to
[upstream iceoryx2](https://github.com/eclipse-iceoryx/iceoryx2) following its
contribution process. See [CONTRIBUTING.md](CONTRIBUTING.md).
