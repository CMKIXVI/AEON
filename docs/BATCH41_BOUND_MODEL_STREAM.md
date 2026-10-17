# Batch 41 — Bound Model Stream Backend

Adds a versioned model-stream backend binding so a stream can retain its storage reader and context.

## Scope

- model stream ABI version 2
- explicit backend binding
- bound reads without repeating callback/context
- NVMe queue reader bound directly to the model stream
- unbound-stream rejection
- backend failure propagation
- strict bounds and position handling

## Validation

Strict C11 validation:

`cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2`

Test result:

`AEON Batch 41 bound model stream test: PASS`

No physical NVMe, DMA, MSI-X, IOMMU, cache-coherency, or hardware validation is claimed.
