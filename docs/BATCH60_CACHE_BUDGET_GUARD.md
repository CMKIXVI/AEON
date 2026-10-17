# Batch 60 — Tensor Cache Budget Guard

Batch 60 adds an explicit byte budget to the AEON tensor cache.

## Contract

- The default budget is `AEON_TENSOR_CACHE_UNLIMITED_BUDGET`.
- `aeon_tensor_cache_set_budget()` rejects a budget below the currently resident byte count and leaves the existing budget unchanged.
- A materialization larger than the configured budget is rejected before cache mutation.
- When an insertion would exceed the budget, the active cache policy selects victims until the new materialization fits.
- The current policy remains LRU; policy dispatch is unchanged.
- `bytes_used`, `peak_bytes_used`, `evictions`, and `inserts` remain consistent across budget-driven eviction.
- Budget checks use subtraction-based arithmetic to avoid `size_t` addition overflow.

## API

```c
int aeon_tensor_cache_set_budget(aeon_tensor_cache_t *cache, size_t budget_bytes);
size_t aeon_tensor_cache_get_budget(const aeon_tensor_cache_t *cache);
```

## Validation

`tests/test_batch60.c` covers default/unlimited budget, a 32-byte budget, LRU victim selection under budget pressure, accounting invariants, rejection of a budget below current residency, and rejection of an oversized materialization without cache mutation.
