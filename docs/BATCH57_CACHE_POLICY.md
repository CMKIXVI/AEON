# Batch 57 — Tensor Cache Policy

## Scope

Batch 57 makes the tensor-cache replacement policy an explicit runtime contract.

## Contract

- `AEON_TENSOR_CACHE_POLICY_LRU` is the initial supported policy.
- Every cache initializes and resets to LRU.
- `aeon_tensor_cache_set_policy()` rejects unsupported policy values.
- `aeon_tensor_cache_get_policy()` exposes the active policy.
- Cache insertion refuses to execute an unsupported policy rather than silently selecting an algorithm.

## Why

The cache already implements least-recently-used eviction. This batch makes that behavior explicit at the ABI boundary so future policies can be added without silently changing existing runtime behavior.

## Validation

`tests/test_batch57.c` verifies initialization, explicit LRU selection, rejection of an unsupported value, and reset behavior.

Full repository and hardware validation are not claimed by this batch.
