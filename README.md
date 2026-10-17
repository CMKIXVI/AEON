# AEON

**AEON — Autonomous Execution & Orchestration Network** is a native, hardware-aware AI runtime/OS project being built in C with a long-term goal of running large open-weight AI models directly on PCs, workstations, servers, AI PCs and embodied/robotic hardware.

> **Current status:** Batch 30 complete. Verified host-test readiness: **70%**. This readiness figure is a development tracking metric, not a claim that 70% of the eventual OS is implemented. Hardware execution is separately validated and is not claimed where it has not been performed.

## Installation & Setup

### 1. Install prerequisites

#### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y git build-essential libssl-dev
```

#### Fedora / RHEL-family

```bash
sudo dnf install -y git gcc make openssl-devel
```

#### macOS

Install Xcode Command Line Tools and Git:

```bash
xcode-select --install
```

If Homebrew is installed:

```bash
brew install git
```

#### Windows

Install Git for Windows and use Git Bash, MSYS2, WSL, or another C11-compatible development environment. The current validated host-test path is primarily Linux/x86-64.

### 2. Clone AEON

```bash
git clone https://github.com/CMKIXVI/AEON.git
cd AEON
```

### 3. Verify the development tools

```bash
git --version
gcc --version
make --version
```

AEON's current host validation requires a C11 compiler and OpenSSL `libcrypto` development support.

### 4. Build

```bash
make clean
make
```

The host validation build uses strict compiler settings:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Warnings are treated as errors.

### 5. Run the tests

```bash
make test
```

The current verified Batch 30 test should report:

```text
AEON Batch 30 semantic payload gate test: PASS
```

### 6. Clean generated files

```bash
make clean
```

### Important: development build vs bootable AEON

The commands above build and validate the **current host-testable AEON development baseline**. They do **not** yet install AEON as a bootable operating system on a physical machine.

The bootable installation path will be introduced only after the real:

```text
UEFI → bootloader → AEON kernel → memory → interrupts/SMP
     → PCIe → NVMe → drivers → runtime
```

path has been implemented and hardware/QEMU validated. AEON does not claim bootable OS functionality before that validation is complete.

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

## Security boundary

AEON's identity layer can require a configured DID identity, VC hash, Ed25519 signature and Proof-of-Friction attestation before a semantic transmission is permitted by the current host-testable policy path.

**Important:** Ed25519 proves possession of a private key. It does **not** by itself prove that a signer is human. Human verification is represented as an explicit configured trust/attestation policy.

The current Web3/IPFS/ORE structures are protocol-neutral data structures. A production blockchain/IPFS network client has **not** been claimed yet.

Likewise, the current eBPF/netfilter code is an enforcement-compatible boundary/model. It does **not** claim that a real kernel eBPF program or netfilter hook has already been attached to a running production kernel.

## Development workflow

1. Work in a feature branch.
2. Keep kernel/driver/security boundaries explicit.
3. Add a deterministic host-side test for every new subsystem boundary.
4. Build with `-Werror`.
5. Do not claim hardware validation without executing on target hardware or a suitable emulator.
6. Update the production-readiness record only after tests pass.
7. Keep failed batches out of the released baseline.

## Production readiness

The current project-level readiness number is a **development tracking metric**, not a claim that 70% of the eventual operating system is implemented.

See:

- `docs/PRODUCTION_READINESS`
- `docs/PRODUCTION_READINESS.md`
- `docs/history/`

## Roadmap

Major remaining work includes:

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
