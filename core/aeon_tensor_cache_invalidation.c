#include "aeon_tensor_cache_invalidation.h"

int aeon_tensor_cache_invalidate(aeon_tensor_cache_t *cache,
                                  const aeon_model_tensor_meta_t *tensor)
{
    unsigned i;
    if (cache == NULL || tensor == NULL) return -1;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        if (cache->entries[i].valid && cache->entries[i].tensor == tensor) {
            cache->entries[i] = (aeon_tensor_cache_entry_t){0};
            return 0;
        }
    }
    return 1;
}

int aeon_tensor_cache_invalidate_all(aeon_tensor_cache_t *cache)
{
    unsigned i;
    if (cache == NULL) return -1;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        cache->entries[i] = (aeon_tensor_cache_entry_t){0};
    }
    return 0;
}
