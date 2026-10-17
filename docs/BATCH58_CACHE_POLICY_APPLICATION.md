# Batch 58 — Tensor Cache Policy Application

Batch 57 introduced an explicit cache-policy ABI with LRU as the supported policy.

Batch 58 verifies the policy contract at the cache lifecycle boundary:

- initialization selects LRU explicitly;
- setting the supported LRU policy succeeds;
- unsupported policy values are rejected;
- a rejected policy does not mutate the active policy;
- reset restores the explicit LRU policy and clears observability counters.

The regression is wired into `make test`.

Full repository, hardware, and real NVMe validation are not claimed by this batch.
