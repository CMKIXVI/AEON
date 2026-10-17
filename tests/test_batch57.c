#include "aeon_tensor_cache.h"

#include <stdio.h>

int main(void)
{
    aeon_tensor_cache_t cache;

    if (aeon_tensor_cache_init(&cache) != 0) return 1;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 2;
    if (aeon_tensor_cache_set_policy(&cache, AEON_TENSOR_CACHE_POLICY_LRU) != 0) return 3;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 4;
    if (aeon_tensor_cache_set_policy(&cache, (aeon_tensor_cache_policy_t)1) != -2) return 5;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 6;
    if (aeon_tensor_cache_reset(&cache) != 0) return 7;
    if (aeon_tensor_cache_get_policy(&cache) != AEON_TENSOR_CACHE_POLICY_LRU) return 8;

    puts("AEON Batch 57 cache policy regression test: PASS");
    return 0;
}
