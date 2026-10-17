# Batch 59 — Tensor Cache Memory Accounting

Batch 59 extends cache observability with explicit resident-memory accounting.

The cache now tracks:

- `bytes_used`: bytes currently owned by valid cache entries;
- `peak_bytes_used`: highest resident byte count observed since reset;
- eviction accounting that removes the replaced entry's bytes before adding the new materialization;
- detach accounting that releases the detached entry's resident bytes.

LRU victim selection is isolated behind a policy dispatch boundary so unsupported policies fail closed rather than silently changing eviction behavior.

The regression verifies four 16-byte tensor materializations, an LRU hit followed by eviction, stable resident capacity, and detach accounting.

Full repository, hardware, and real NVMe validation are not claimed by this batch.
