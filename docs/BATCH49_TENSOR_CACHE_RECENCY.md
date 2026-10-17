# Batch 49 — Tensor Cache Recency

## Scope

Adds explicit LRU-style recency tracking to the bounded tensor cache.

## Changes

- Adds `aeon_tensor_cache_access()` to refresh an entry's recency.
- Cache insertion refreshes recency for duplicate entries.
- Cache eviction selects the least recently accessed valid entry.
- Handles the unsigned recency clock wrapping without leaving stale maximum timestamps.
- Adds a focused regression test covering four-entry fill, access refresh, eviction, and reinsertion.

## Validation boundary

The Batch 49 test is strict C11 with `-Wall -Wextra -Wpedantic -Werror`.

This batch does not claim full repository validation, hardware validation, NVMe execution, GPU execution, or model inference.
