# Batch 50 — Tensor Materialization Integrity

## Scope

Strengthens tensor materialization validation by requiring the bound tensor metadata to pass the tensor-layout validator before materialization is accepted.

## Changes

- Validate tensor dtype/rank/shape/byte consistency during materialization validation.
- Validate tensor layout before loading bytes from a tensor binding.
- Add regression coverage for successful materialization and metadata/byte mismatch rejection.

## Validation boundary

The Batch 50 test is strict C11 with `-Wall -Wextra -Wpedantic -Werror`.

Full repository, hardware, NVMe, GPU, and model-inference validation are not claimed.
