#include "aeon_tensor_cache.h"

#include <stdio.h>

int main(void)
{
    aeon_tensor_cache_t cache;
    aeon_tensor_cache_stats_t stats;

    if (aeon_tensor_cache_init(&cache) != 0) return 1;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 2;
    if (aeon_tensor_cache_set_policy(&cache, AEON_TENSOR_CACHE_POLICY_LRU) != 0) return 3;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 4;
    if (aeon_tensor_cache_set_policy(&cache, (aeon_tensor_cache_policy_t)99) != -2) return 5;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 6;
    if (aeon_tensor_cache_reset(&cache) != 0) return 7;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 8;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0) return 9;
    if (stats.hits != 0u || stats.misses != 0u || stats.inserts != 0u ||
        stats.evictions != 0u || stats.stale_entries != 0u ||
        stats.bytes_used != 0u || stats.peak_bytes_used != 0u) return 10;

    puts("AEON Batch 58 cache policy application regression test: PASS");
    return 0;
}
