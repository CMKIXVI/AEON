#ifndef AEON_TENSOR_CACHE_H
#define AEON_TENSOR_CACHE_H

#include <stddef.h>
#include "aeon_tensor_materialization.h"
#include "aeon_tensor_cache_policy.h"

#define AEON_TENSOR_CACHE_MAX_ENTRIES 4u
#define AEON_TENSOR_CACHE_UNLIMITED_BUDGET ((size_t)-1)

typedef struct {
    size_t hits;
    size_t misses;
    size_t inserts;
    size_t evictions;
    size_t stale_entries;
    size_t bytes_used;
    size_t peak_bytes_used;
} aeon_tensor_cache_stats_t;

typedef struct {
    aeon_tensor_materialization_t *materialization;
    const aeon_model_tensor_meta_t *tensor;
    size_t bytes;
    unsigned valid;
    unsigned age;
} aeon_tensor_cache_entry_t;

struct aeon_tensor_cache {
    aeon_tensor_cache_entry_t entries[AEON_TENSOR_CACHE_MAX_ENTRIES];
    unsigned clock;
    aeon_tensor_cache_stats_t stats;
    aeon_tensor_cache_policy_t policy;
    size_t budget_bytes;
};

int aeon_tensor_cache_init(aeon_tensor_cache_t *cache);
int aeon_tensor_cache_reset(aeon_tensor_cache_t *cache);
const aeon_tensor_cache_entry_t *aeon_tensor_cache_find(const aeon_tensor_cache_t *cache,
                                                        const aeon_model_tensor_meta_t *tensor);
const aeon_tensor_cache_entry_t *aeon_tensor_cache_access(aeon_tensor_cache_t *cache,
                                                          const aeon_model_tensor_meta_t *tensor);
int aeon_tensor_cache_put(aeon_tensor_cache_t *cache,
                          aeon_tensor_materialization_t *materialization);
int aeon_tensor_cache_detach(aeon_tensor_cache_t *cache,
                             aeon_tensor_materialization_t *materialization);
int aeon_tensor_cache_validate(const aeon_tensor_cache_t *cache);
int aeon_tensor_cache_get_stats(const aeon_tensor_cache_t *cache,
                                aeon_tensor_cache_stats_t *stats);
int aeon_tensor_cache_record_stale(aeon_tensor_cache_t *cache);
int aeon_tensor_cache_set_policy(aeon_tensor_cache_t *cache,
                                 aeon_tensor_cache_policy_t policy);
aeon_tensor_cache_policy_t aeon_tensor_cache_get_policy(const aeon_tensor_cache_t *cache);
int aeon_tensor_cache_set_budget(aeon_tensor_cache_t *cache, size_t budget_bytes);
size_t aeon_tensor_cache_get_budget(const aeon_tensor_cache_t *cache);

#endif
