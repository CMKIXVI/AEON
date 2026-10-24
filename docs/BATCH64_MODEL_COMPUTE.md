# Batch 64 — Model Compute

Adds host-testable transformer primitives required by the AEON model runtime:
- numerically stabilized softmax
- RMSNorm
- rotary positional embedding
- scaled dot-product attention
- SwiGLU

The kernels are deliberately small, deterministic C11 boundaries so they can later be connected to tensor materialization, KV-cache storage, SIMD backends, and the transformer graph.

This batch does not claim full GGUF execution, SIMD acceleration, GPU execution, or hardware validation.
