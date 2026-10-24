# Batch 62 — Runtime Integration Foundations

This batch imports the supplied runtime-extension design into AEON without falsely promoting external or hardware-dependent stubs to production status.

## Network

`experimental/aeon_live_transport.c` records the intended path:

`PCIe NIC -> lwIP -> TCP -> BearSSL -> HTTPS`

The stable AEON boundary is `include/aeon_uefi_network.h`. The live backend is enabled only with `AEON_UEFI_LIVE_NETWORK` and requires the UEFI NIC driver, TCP/IP stack, certificate store, SNI, TLS BIO, and HTTP implementation.

## Model runtime

`experimental/aeon_transformer_graph.c` binds transformer execution to the existing `aeon_model_index_t` rather than creating a second weight-storage system. The intended execution order is:

`Embedding -> RMSNorm -> RoPE/QKV -> KV cache -> Attention -> Residual -> SwiGLU -> Residual -> Final Norm -> LM head`

Actual tensor execution remains a backend task and is not claimed complete by this batch.

## x86 kernel

`experimental/aeon_x86_kernel.c` provides a small host-visible IDT/PML4 boundary and an actual `lidt` operation for x86-64 builds. LAPIC timer programming, ISR entry/exit stubs, scheduler ownership, page-table construction, CR3 switching, TLB management, and UEFI ExitBootServices transition remain separate implementation work.

## Status

These files are intentionally outside the default Makefile test graph. This prevents optional UEFI/lwIP/BearSSL dependencies and privileged kernel operations from contaminating the host-testable AEON core. No hardware validation is claimed.
