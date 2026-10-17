# Batch 51 — Cache-Backed Tensor Materialization

## Scope

Connects the bounded tensor cache to tensor materialization so repeated reads of the same tensor can reuse cached bytes instead of re-reading the model stream.

## Changes

- Adds `aeon_tensor_cache_materialize()`.
- Cache miss loads through the existing tensor binding and inserts the validated materialization.
- Cache hit copies the cached bytes into the requested destination and refreshes recency.
- Updates Batch 47/49 fixtures for the stricter tensor-layout validation introduced by Batch 50.
- Adds a regression test proving the second materialization does not invoke the backing stream reader.

## Validation boundary

The Batch 51 test is strict C11 with `-Wall -Wextra -Wpedantic -Werror`.

Full repository, hardware, NVMe, GPU, and model-inference validation are not claimed.
