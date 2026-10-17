#ifndef AEON_TENSOR_CACHE_POLICY_H
#define AEON_TENSOR_CACHE_POLICY_H

typedef struct aeon_tensor_cache aeon_tensor_cache_t;

typedef enum {
    AEON_TENSOR_CACHE_POLICY_LRU = 0
} aeon_tensor_cache_policy_t;

int aeon_tensor_cache_set_policy(aeon_tensor_cache_t *cache,
                                 aeon_tensor_cache_policy_t policy);
aeon_tensor_cache_policy_t aeon_tensor_cache_get_policy(const aeon_tensor_cache_t *cache);

#endif
