# Batch 63 — Cognitive Compute Core

Implements the first integrated vertical slice from the AEON 14-layer specification:

- 2048-bit VSA binding, majority bundling, circular permutation and Hamming distance.
- 1024-node Izhikevich membrane integration with dopamine/norepinephrine state and branching-ratio telemetry.
- Deterministic free-energy / expected-free-energy state update.
- Q4 nibble unpacking and scalar quantized GEMV boundary.

This is portable host-testable C11 code. It does not claim AVX-512/NEON execution or physical SNN hardware validation.
