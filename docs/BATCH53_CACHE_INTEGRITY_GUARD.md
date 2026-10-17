# Batch 53 — Tensor Cache Integrity Guard

Batch 53 hardens cache-backed tensor materialization against stale borrowed materializations.

## Behavior

On a cache hit, AEON now validates the cached materialization before copying bytes. If the cached object has been reset, has invalid layout/byte state, or no longer refers to the requested tensor, the cache entry is detached and the tensor is reloaded from the backing stream.

The cache remains bounded and non-owning. Callers that intentionally reset a cached materialization should use `aeon_tensor_cache_detach()` first; the integrity guard is a defensive fallback against stale state.

## Regression coverage

The Batch 53 test materializes a tensor, resets the cached source materialization without detaching it, and verifies that the next request performs a backing read and produces correct bytes instead of copying stale state.

Full repository and hardware validation are not claimed.
