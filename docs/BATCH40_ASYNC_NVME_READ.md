# Batch 40 — Asynchronous NVMe Read Lifecycle

Batch 40 separates NVMe model reads into explicit submit and completion phases.

## Added

- `aeon_nvme_queue_read_start()` submits a bounded, LBA-aligned request and records its CID.
- `aeon_nvme_queue_read_complete()` polls the CID and transitions the request to COMPLETE or FAILED.
- The existing synchronous request API remains as a compatibility wrapper.
- Request state and destination buffer are retained across the asynchronous boundary.
- Strict C11 validation and a fake queue lifecycle test are included.

## Validation

Local validation performed before GitHub push:

```text
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
AEON Batch 40 asynchronous NVMe read lifecycle test: PASS
```

## Boundary

This batch does not claim physical NVMe controller execution, DMA, MSI-X delivery, IOMMU, cache coherency, or hardware validation. The submit/poll callbacks are the explicit hardware integration boundary.
