# Batch 52 — Tensor Cache Lifetime Contract

## Scope

Makes tensor-cache ownership explicit. Cache entries borrow materialization objects; they must be detached before the backing materialization is reset or destroyed.

## Changes

- Adds an explicit cache detach operation for a materialization.
- Detach clears the cache entry instead of leaving a stale materialization pointer.
- Cache reset/invalidation remains available for bulk lifetime transitions.
- Adds a focused regression test for detach-before-reuse.

## Ownership rule

`aeon_tensor_cache_t` does not own the memory referenced by `aeon_tensor_materialization_t`.
The caller owns each materialization and is responsible for calling the detach operation before resetting, reusing, or destroying a materialization that is currently cached.

This is an intentional bounded-runtime contract; cache-owned allocation is deferred until AEON's allocator/ownership subsystem is established.

## Validation boundary

Strict C11 validation is required. Full repository, hardware, NVMe, GPU, and model-inference validation are not claimed.
