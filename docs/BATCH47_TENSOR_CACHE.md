# Batch 47 — Tensor Cache

Adds a bounded, caller-owned tensor cache boundary.

- fixed maximum of four entries
- no implicit allocation
- tensor identity and byte count tracked
- duplicate insertion is idempotent
- oldest entry selected when full
- explicit reset

This batch does not perform tensor decoding, dequantization, GPU execution, or hardware validation.
