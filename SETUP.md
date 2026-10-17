# AEON Setup Guide

## 1. Install dependencies

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y git build-essential libssl-dev
```

### Fedora / RHEL

```bash
sudo dnf install -y git gcc make openssl-devel
```

## 2. Clone

```bash
git clone https://github.com/CMKIXVI/AEON.git
cd AEON
```

## 3. Build and run the current host validation

```bash
make clean
make test
```

The current release gate is Batch 30. It compiles with strict C11 warnings-as-errors and links against OpenSSL `libcrypto` for Ed25519/SHA-256 verification.

## 4. Manual compiler invocation

If `make` is unavailable, the current Batch 30 test can be built with:

```bash
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude \
  core/aeon_identity.c \
  core/aeon_identity_crypto.c \
  core/aeon_syscall.c \
  core/aeon_friction.c \
  core/aeon_io_gate.c \
  core/aeon_process_gate.c \
  core/aeon_ebpf_boundary.c \
  core/aeon_kernel_device.c \
  core/aeon_net_policy.c \
  core/aeon_kernel_adapter.c \
  core/aeon_semantic_frame.c \
  core/aeon_semantic_gate.c \
  tests/test_batch30.c \
  -o aeon_batch30_test -lcrypto

./aeon_batch30_test
```

## 5. Development notes

The repository contains several subsystem generations because AEON is being built incrementally in validated batches. Some files are architecture models or host-testable boundaries rather than hardware drivers that can already execute on physical hardware.

Before using AEON on hardware, check the relevant validation document in `docs/` and do not assume that a host test proves physical device operation.

## 6. Adding a new batch

For each new batch:

```text
Implement -> compile with -Werror -> run deterministic tests -> inspect failures -> fix -> rerun -> release only on PASS
```

Record:

- scope implemented
- tests executed
- hardware validation performed or not performed
- known limitations
- production-readiness percentage

Never increase readiness for a failed or unvalidated batch.
