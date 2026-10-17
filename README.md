<p align="center">
  <img src="./aeon-banner.png" alt="AEON — Autonomous Engine, Orchestrator & Node">
</p>

# AEON — Autonomous Engine, Orchestrator & Node

AEON is a native AI runtime/OS foundation being developed in C for autonomous execution, model runtime, compute, storage, networking, hardware control, robotics and secure AI I/O.

## Current status

**Batch 33 — 76% production readiness**

Batch 33 adds a bounded, alignment-aware runtime memory foundation with explicit region state, overflow-safe allocation, reset/reuse lifecycle and capacity reporting. It is designed as the memory substrate for future native model-runtime and cognitive-execution components.

The 76% figure is an engineering readiness estimate for the implemented scope, not a claim that AEON is a finished production operating system. Physical kernel/eBPF/netfilter attachment, hardware network interception, physical memory management, IOMMU/DMA mapping and other hardware-level validation remain future work.

## Build and test

```bash
make clean
make test
```

Strict baseline:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Expected results:

```text
AEON Batch 31 authenticated semantic transmission test: PASS
AEON Batch 32 semantic wire codec test: PASS
AEON Batch 33 runtime memory test: PASS
```

## Architecture

```text
AEON
├── Cognitive Engine
├── Model Runtime
├── Compute Engine
├── Runtime Memory
├── Storage Engine
├── System / UEFI / Kernel
├── Network
├── Interface / Widgets
├── Tool Bus
└── Robotics / Embodied / VLA
```

Security work includes self-sovereign identity, semantic S-P-O transmission gates, cryptographic verification and Proof-of-Friction policy boundaries.

## Repository layout

- `core/` — AEON runtime/security foundations
- `drivers/` — hardware and NVMe foundations
- `kernel/` — kernel-facing boundaries
- `include/` — public C interfaces
- `tests/` — strict host-testable validation
- `docs/` — engineering scope and readiness records

## Important boundary

Host-testable code is deliberately distinguished from claims of a bootable/hardware-validated OS. A component is not considered hardware-complete until it has been validated against its real execution environment.

## Roadmap

Next engineering stages continue from runtime memory toward native model-runtime memory management, tensor/buffer lifecycle, physical memory integration, kernel I/O enforcement, hardware network interception, cognitive execution and robotics/embodied AI integration.
