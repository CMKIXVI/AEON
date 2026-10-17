# Batch 37 — Model Storage Backend

Adds a bounded in-memory model storage backend to the model streaming ABI.

## Scope
- exact storage-size binding
- callback-backed sequential reads
- seek-aware reads
- bounds enforcement
- storage-size mismatch rejection

This is a host-testable storage adapter. It is intentionally not an NVMe hardware driver and does not claim DMA, queue, interrupt, IOMMU, or physical-device validation.
