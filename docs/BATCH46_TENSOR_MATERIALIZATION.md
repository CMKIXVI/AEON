# Batch 46 — Tensor Materialization

Adds a bounded materialization/cache boundary between a validated tensor binding and caller-owned memory.

## Guarantees
- Caller supplies the buffer and capacity.
- No implicit allocation or unbounded cache growth.
- Tensor metadata is inherited from the validated binding.
- Materialization rejects buffers smaller than the tensor byte range.
- Reset clears the active tensor and byte count without freeing caller memory.
- Validation tracks the materialized tensor and exact byte count.

## Not included
- Tensor decoding or dequantization.
- GPU/device memory management.
- Automatic eviction or multi-tensor cache policy.
- Model inference.
- Physical NVMe or hardware validation.
