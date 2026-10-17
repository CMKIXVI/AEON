#ifndef AEON_TENSOR_CACHE_INVALIDATION_H
#define AEON_TENSOR_CACHE_INVALIDATION_H

#include "aeon_tensor_cache.h"

int aeon_tensor_cache_invalidate(aeon_tensor_cache_t *cache,
                                  const aeon_model_tensor_meta_t *tensor);
int aeon_tensor_cache_invalidate_all(aeon_tensor_cache_t *cache);

#endif
