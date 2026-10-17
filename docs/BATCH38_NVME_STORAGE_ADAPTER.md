# Batch 38 — NVMe Storage Adapter

Adds an NVMe-shaped LBA storage adapter between the model streaming ABI and a storage backend.

## Implemented
- LBA-size validation
- capacity validation
- byte-range to LBA-range translation
- alignment enforcement
- bounded reads
- model-stream binding
- host-testable fake-device coverage

## Boundary
This is an adapter contract, not physical NVMe execution. It does not claim controller discovery, queue submission, DMA, MSI-X, IOMMU, cache coherency, or hardware validation.

Strict target: `-std=c11 -Wall -Wextra -Wpedantic -Werror -O2`.
