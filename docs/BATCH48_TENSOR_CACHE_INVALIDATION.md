# Batch 48 — Tensor Cache Invalidation

Adds explicit invalidation for individual tensor cache entries and the complete cache.

## Guarantees

- No allocation or ownership transfer.
- Individual invalidation is deterministic.
- Missing-entry invalidation returns `1`.
- Full invalidation clears all entries.
- Cache materialization storage remains caller-owned.

This batch does not implement model decoding, dequantization, GPU execution, or hardware cache coherency.
