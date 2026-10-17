# Batch 61 — Tensor Cache Admission / Residency Validation

Batch 61 adds an explicit validator for the tensor-cache residency contract.

## Invariants

- `bytes_used` equals the sum of resident valid entry sizes.
- Resident bytes never exceed the configured cache budget.
- Every valid entry has a materialization, tensor, non-zero size, and recency age.
- The materialization metadata agrees with the cache entry metadata.
- A tensor may not occupy multiple cache slots.
- Invalid/empty slots contain no stale ownership metadata.
- The active cache policy remains LRU.

## API

```c
int aeon_tensor_cache_validate(const aeon_tensor_cache_t *cache);
```

The validator is intentionally side-effect free and returns non-zero on any residency invariant violation.

## Validation

`tests/test_batch61.c` exercises validation after initial admission, budget-driven LRU eviction, and explicit detach operations.
