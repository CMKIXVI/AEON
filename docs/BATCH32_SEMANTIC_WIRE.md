# Batch 32 — Semantic Wire Transport

## Objective

Move the authenticated semantic frame from an in-memory C structure toward a deterministic wire representation suitable for future native network and IPC transports.

## Implemented

- fixed wire magic and version
- explicit big-endian integer encoding
- bounded length-prefixed strings
- authenticated identity fields
- Proof-of-Friction challenge and attestation fields
- IPFS CID field
- strict decode bounds and exact-consumption checks
- frame validation after decode
- round-trip test
- process-gate ABI hardening: explicit conversion between ioctl and internal submit structures

## Validation

```text
AEON Batch 31 authenticated semantic transmission test: PASS
AEON Batch 32 semantic wire codec test: PASS
```

The codec is host-testable and transport-neutral. It does not claim physical network interception or kernel attachment.
