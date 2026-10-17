# Batch 43 — Model Manifest Parser + Tensor Loader

Adds a bounded text manifest parser and tensor-loading bridge on top of the model tensor index and bound model stream.

## Manifest format

Each record is:

```text
name|offset|bytes|dtype|rank|dim0,dim1,...
```

Comments beginning with `#` and blank lines are ignored.

Supported dtypes are `F32`, `BF16`, `F16`, `FP8_E4M3`, and `I8`.

## Safety

- bounded manifest allocation
- strict field count
- decimal integer validation
- rank/dimension validation
- duplicate and overlap rejection delegated to the model index
- model-range validation
- destination-buffer size validation before tensor load
- no tensor bytes are copied during parsing

## Loading

`aeon_model_manifest_load_tensor()` resolves a tensor by name, seeks the bound model stream to its byte offset, and reads exactly the indexed byte range into the caller's buffer.

## Validation

Focused strict C11 validation was run with:

```text
-std=c11 -Wall -Wextra -Wpedantic -Werror -O2
```

Result:

```text
AEON Batch 43 model manifest/loader test: PASS
```

Full repository validation remains separate from this focused test because the local environment cannot resolve `github.com` for a complete repository clone.

## Not claimed

This batch does not parse a vendor-specific production model container, execute inference, or claim physical NVMe/DMA/hardware validation.
