# Batch 65 — Cognitive Engine

This batch adds a deterministic C11 cognitive-engine vertical slice connecting symbolic retrieval, causal intervention, active-inference action selection, episodic consolidation, and global-workspace selection.

## Implemented

- VSA-style dictionary cleanup using normalized integer-vector similarity.
- Causal graph edges and a Pearl-style `do(X)` intervention query over registered edges.
- Active-inference action scoring from prediction error and uncertainty, returning an explicit expected-free-energy score and confidence.
- Bounded episodic memory with Fisher-weighted consolidation penalty.
- Global workspace publication with salience-based selection and bounded replacement.

## Validation boundary

`tests/test_batch65.c` exercises each API and is wired into `make test` as `aeon_batch65_test`.

This is a deterministic cognitive-runtime foundation, not a claim of general intelligence, full Paninian grammar, full Pearl SCM semantics, or biologically faithful memory. Hardware execution and full-repository test execution remain separate validation stages.
