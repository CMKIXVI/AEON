# Batch 56 — Tensor Cache Observability

Batch 56 adds bounded cache statistics for runtime observability.

## Counters

- `hits`: successful cache lookups through `aeon_tensor_cache_access()`.
- `misses`: unsuccessful cache lookups through `aeon_tensor_cache_access()`.
- `inserts`: new materializations inserted into the cache.
- `evictions`: valid cache entries replaced because all slots were occupied.
- `stale_entries`: cached materializations rejected by the integrity guard.

Statistics are cumulative for the lifetime of the cache instance and are reset by
`aeon_tensor_cache_reset()`.

## API

`aeon_tensor_cache_get_stats()` returns an atomic snapshot of the bounded
statistics structure at the current API boundary. It does not expose internal
cache entries or permit callers to mutate accounting state.

## Validation

The Batch 56 regression test verifies zeroed initial counters, misses and
inserts during cold materialization, a valid cache hit without a backing read,
eviction accounting, and stale-entry recovery after materialization reset.

This batch validates the host-testable accounting boundary only; it does not
claim hardware cache-coherency, concurrent multi-core accounting, or physical
NVMe execution.
