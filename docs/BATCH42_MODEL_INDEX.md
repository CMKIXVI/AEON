# Batch 42 — Model Tensor Index

## Scope

Adds a bounded, validated tensor metadata index for large model weights.

The index records:

- tensor name
- byte offset in the model image
- byte length
- supported storage dtype
- rank
- tensor shape

## Safety

- fixed ABI magic/version
- bounded tensor count and name length
- rank and dimension validation
- duplicate-name rejection
- model-range validation
- overlap rejection using overflow-safe offset comparisons
- lookup by tensor name

## Supported metadata dtypes

- F32
- BF16
- F16
- FP8 E4M3
- I8

## Validation

Focused strict C11 validation was run locally with:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Result:

```text
AEON Batch 42 model index test: PASS
```

The full repository suite was not used to increase readiness because the local environment could not resolve `github.com` when attempting to clone the complete repository.

## Not claimed

This batch does not yet parse a production model container, load tensor bytes, perform inference, or validate a physical NVMe controller.
