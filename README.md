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
| Maintainer topic branches and pull requests | Review of fork maintenance changes |
| `holoscan-sdk/<lane>/<upstream-ref>-<base-sha>/rN` | Optional reproducible patch-series checkpoints |

Fork maintainers develop directly on GitHub. Maintenance branches may advance
before an SDK release is qualified; their names do not assert release readiness. Consumer
build definitions pin a full commit SHA. Release tags identify the exact source
selected for a validated public SDK release and are never moved or reused.
Public preview tags retain the SDK's exact preview-tag spelling.

Patch-series checkpoint branches retain their published commits: append changes
without rewriting history, or use the next `rN` revision when reconstructing a
series. SDK development can also carry ordered patches above a public upstream
or fork commit. Remove a patch once it is included in the selected baseline.

## Contributions

This fork does not accept external contributions. Submit improvements to
[upstream iceoryx2](https://github.com/eclipse-iceoryx/iceoryx2) following its
contribution process. See [CONTRIBUTING.md](CONTRIBUTING.md).
