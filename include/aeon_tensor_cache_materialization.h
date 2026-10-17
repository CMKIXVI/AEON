#ifndef AEON_TENSOR_CACHE_MATERIALIZATION_H
#define AEON_TENSOR_CACHE_MATERIALIZATION_H

#include "aeon_tensor_binding.h"
#include "aeon_tensor_cache.h"

int aeon_tensor_cache_materialize(aeon_tensor_cache_t *cache,
                                  aeon_tensor_materialization_t *materialization,
                                  aeon_tensor_binding_t *binding);

#endif
