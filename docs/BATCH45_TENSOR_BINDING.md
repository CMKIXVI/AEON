# Batch 45 — Tensor Binding

Adds a validated bridge from model metadata to a bound model stream.

## Scope

- Resolve tensor metadata by name.
- Revalidate the complete model index against the bound stream size.
- Validate tensor logical layout using Batch 44.
- Reject offsets and sizes that cannot be represented safely by `size_t`.
- Provide bounded seek/read access within one tensor's byte range.
- Keep the underlying model stream position synchronized for each read.

## Safety boundary

This batch does not decode tensor values, dequantize weights, allocate GPU memory, execute inference, or claim physical NVMe/DMA validation.
