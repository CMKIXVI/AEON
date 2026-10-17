# AEON

**AEON — Autonomous Execution & Orchestration Network** is a native, hardware-aware AI runtime/OS project being built in C/C with a long-term goal of running large open-weight AI models directly on PC, workstation, server, AI-PC and embodied/robotic hardware.

> **Current status:** Batch 30 complete. Verified host-test readiness: **70%**. The repository contains the consolidated source from the verified AEON batches through Batch 30. Hardware execution is still a separate validation stage and is explicitly not claimed where it has not been performed.

## Architecture

AEON is being developed as a native stack rather than as an application sitting on top of Linux:

```text
AEON
├── System / Boot / Kernel foundation
├── Memory / CPU / Interrupts / SMP
├── PCI / PCIe / ACPI / MMIO
├── NVMe / DMA / Queue / Completion runtime
├── Compute / CPU feature detection / SIMD
├── Identity / SSI / DID / Ed25519 verification
├── Proof-of-Friction policy
├── Semantic S-P-O transmission gate
├── Process / IO / network policy enforcement
├── eBPF/netfilter-compatible enforcement boundary
├── Model streaming foundation
├── Cognitive Engine (planned / expanding)
├── Model Runtime (planned / expanding)
├── Tool Bus (planned / expanding)
├── PC hardware profiles
└── Embodied / VLA / ROS-compatible layer (planned / expanding)
```

AEON is **PC-first and robotics-capable**. Robotics functionality is intended to use AEON-native real-time services while providing compatibility with useful ROS 2 concepts and protocols. AEON does not treat Linux as its architectural foundation.

## Repository layout

```text
arch/       x86-64 architecture code
boot/       boot/UEFI boundary code
compute/    compute and ISA selection
core/       core runtime and identity/security gates
drivers/    PCI, NVMe, MMIO, DMA and device code
hardware/   hardware detection
include/    public C headers
kernel/     kernel-facing services and interrupt paths
mm/         memory-management foundations
runtime/    model/data streaming foundations
tests/      host-side validation tests
docs/       architecture, validation and batch history
uefi/       UEFI implementation boundary
```

## Current verified scope

The latest completed batch is **Batch 30**. The verified work includes:

- UEFI/boot and handoff foundations
- page-table and memory-management models
- CPU/interrupt/SMP foundations
- PCI/PCIe discovery and ECAM
- PCI BAR/resource discovery
- MSI/MSI-X and interrupt routing models
- CPU feature detection and compute dispatch foundations
- NVMe controller, queue, request and completion models
- namespace discovery/parsing/enumeration
- model byte-range streaming foundation
- DMA/PRP request modeling
- DID registry and Ed25519 verification
- semantic triple hashing
- Proof-of-Friction challenge/attestation binding
- process-level IO policy
- semantic payload integrity validation
- network TX/RX policy boundaries
- kernel-device/ioctl boundary model
- eBPF-compatible event classification boundary

### Security boundary

AEON's identity layer can require a configured DID identity, VC hash, Ed25519 signature and Proof-of-Friction attestation before a semantic transmission is permitted by the current host-testable policy path.

**Important:** Ed25519 proves possession of a private key. It does **not** by itself prove that a signer is human. Human verification is represented as an explicit configured trust/attestation policy.

The current Web3/IPFS/ORE structures are protocol-neutral data structures. A production blockchain/IPFS network client has **not** been claimed yet.

Likewise, the current eBPF/netfilter code is an enforcement-compatible boundary/model. It does **not** claim that a real kernel eBPF program or netfilter hook has already been attached to a running production kernel.

## Build requirements

The current host validation path uses:

- C11 compiler (`gcc` or `clang`)
- `make`
- OpenSSL development headers/library (`libcrypto`)
- x86-64 host recommended for the architecture-oriented portions

Ubuntu/Debian example:

```bash
sudo apt update
sudo apt install build-essential libssl-dev
```

Fedora/RHEL-family example:

```bash
sudo dnf install gcc make openssl-devel
```

## Quick start

Clone the repository:

```bash
git clone https://github.com/CMKIXVI/AEON.git
cd AEON
```

Run the currently verified Batch 30 host test:

```bash
make test
```

The expected result is:

```text
AEON Batch 30 semantic payload gate test: PASS
```

Clean generated test binaries:

```bash
make clean
```

## Strict build

The project intentionally uses strict warnings for the host validation path:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Do not treat a warning-producing build as production-ready. A batch is considered released only after its defined validation path passes.

## Development workflow

1. Work in a feature branch.
2. Keep kernel/driver/security boundaries explicit.
3. Add a host-side deterministic test for every new subsystem boundary.
4. Build with `-Werror`.
5. Do not claim hardware validation without executing on the target hardware or a suitable hardware emulator.
6. Update the production-readiness record only after tests pass.
7. Keep failed batches out of the released baseline.

## Production readiness

The current project-level readiness number is a **development tracking metric**, not a claim that 70% of the eventual operating system is implemented.

See:

- `docs/PRODUCTION_READINESS`
- `docs/PRODUCTION_READINESS.md`
- `docs/history/`

## Roadmap

The major remaining work includes:

- real boot-to-kernel execution on hardware/QEMU
- physical APIC/interrupt/SMP validation
- physical PCIe/NVMe controller execution
- IOMMU/cache-coherency/DMA correctness
- network stack and physical network enforcement
- real kernel eBPF/netfilter integration where applicable
- GPU/accelerator runtime
- large-model loader and tensor index
- tokenizer and inference engine
- KV cache and streaming generation
- MoE/DSA execution for the target large model
- cognitive engine: goals, memory, planning, reasoning, reflection and autonomous execution
- tool bus and capability security
- PC hardware profiles
- embodied/VLA runtime and real-time robotics services
- ROS 2 compatibility interfaces
- integration, fuzzing, stress testing and long-duration hardware validation

## License

No open-source license has been declared in this repository yet. Until a license is added, assume the repository contents are **all rights reserved**.

## Project

Repository: https://github.com/CMKIXVI/AEON
