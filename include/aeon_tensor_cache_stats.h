#ifndef AEON_TENSOR_CACHE_STATS_H
#define AEON_TENSOR_CACHE_STATS_H

#include <stddef.h>

#include "aeon_tensor_cache.h"

typedef struct {
    size_t hits;
    size_t misses;
    size_t inserts;
    size_t evictions;
    size_t stale_entries;
} aeon_tensor_cache_stats_t;

void aeon_tensor_cache_stats_reset(aeon_tensor_cache_stats_t *stats);
void aeon_tensor_cache_stats_snapshot(const aeon_tensor_cache_t *cache,
                                      aeon_tensor_cache_stats_t *stats);

#endif
