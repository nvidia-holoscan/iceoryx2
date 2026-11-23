<!--
SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
SPDX-License-Identifier: Apache-2.0
-->

# Security Policy

## Reporting a vulnerability

For vulnerabilities affecting NVIDIA-maintained changes or their Holoscan SDK
integration, follow the [NVIDIA Product Security reporting guidance](https://www.nvidia.com/en-us/security/)
or contact [NVIDIA PSIRT](mailto:psirt@nvidia.com).

For vulnerabilities in upstream iceoryx2, follow the
[upstream security policy](https://github.com/eclipse-iceoryx/iceoryx2/blob/main/SECURITY.md)
and the [Eclipse Foundation Vulnerability Reporting Policy](https://www.eclipse.org/security/policy/).

Use private reporting channels for vulnerability details rather than public
issues, discussions, or pull requests. Include the affected fork commit or tag,
Holoscan SDK version when applicable, platform, build configuration, impact,
and reproduction steps. Follow the chosen reporting channel's instructions for
sharing sensitive material.

## Repository scope

This fork carries iceoryx2 source changes used by NVIDIA Holoscan SDK. The
`nvidia` branch contains fork documentation; it has no executable dependency
implementation, service endpoints, authentication system, or application data
store. Its security-relevant content includes source-selection guidance and
links to maintenance and reporting locations.

Implementation code resides on the source branches. Assess that code against
its selected source commit and application context; a review of the landing
branch does not cover the runtime or its integrations.

## Source and release selection

Use the full dependency commit recorded by the consuming SDK build or OE recipe.
Maintenance branches may contain changes awaiting qualification. Their presence
does not establish a security-support period or release readiness. Report the
exact affected source selection so maintainers can identify the relevant code.
