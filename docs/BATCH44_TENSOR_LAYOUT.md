# Batch 44 — Tensor Layout Validation

Adds a small, deterministic tensor-layout layer between model metadata and future tensor materialization/inference paths.

## Scope

- maps supported AEON model dtypes to element size
- computes element count with `uint64_t` overflow checks
- computes expected byte size from rank, shape, and dtype
- rejects zero dimensions and invalid ranks
- validates that indexed tensor bytes exactly match its logical layout

## Validation

Focused strict C11 compilation:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Result:

```text
AEON Batch 44 tensor layout test: PASS
```

The complete repository suite remains separately gated because the local container cannot resolve `github.com` for a full checkout.

## Not claimed

This batch does not perform tensor decoding, dequantization, GPU execution, or physical hardware validation.
