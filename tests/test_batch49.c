#include "aeon_tensor_cache.h"
#include <stdint.h>
#include <stdio.h>

static int init_materialization(aeon_tensor_materialization_t *materialization,
                                aeon_model_tensor_meta_t *tensor,
                                uint8_t *buffer)
{
    tensor->dtype = AEON_MODEL_DTYPE_F32;
    tensor->rank = 1u;
    tensor->shape[0] = 2u;
    tensor->bytes = 8u;
    if (aeon_tensor_materialization_init(materialization, buffer, 8u) != 0) return -1;
    materialization->tensor = tensor;
    materialization->bytes = 8u;
    return 0;
}

int main(void)
{
    aeon_tensor_cache_t cache;
    aeon_model_tensor_meta_t tensors[5] = {0};
    aeon_tensor_materialization_t materializations[5];
    uint8_t buffers[5][8] = {{0}};

    if (aeon_tensor_cache_init(&cache) != 0) return 1;
    if (init_materialization(&materializations[0], &tensors[0], buffers[0]) != 0 ||
        init_materialization(&materializations[1], &tensors[1], buffers[1]) != 0 ||
        init_materialization(&materializations[2], &tensors[2], buffers[2]) != 0 ||
        init_materialization(&materializations[3], &tensors[3], buffers[3]) != 0 ||
        init_materialization(&materializations[4], &tensors[4], buffers[4]) != 0) return 2;
    if (aeon_tensor_cache_put(&cache, &materializations[0]) != 0 ||
        aeon_tensor_cache_put(&cache, &materializations[1]) != 0 ||
        aeon_tensor_cache_put(&cache, &materializations[2]) != 0 ||
        aeon_tensor_cache_put(&cache, &materializations[3]) != 0) return 3;

    if (aeon_tensor_cache_access(&cache, &tensors[0]) == NULL) return 4;
    if (aeon_tensor_cache_put(&cache, &materializations[4]) != 0) return 5;
    if (aeon_tensor_cache_find(&cache, &tensors[0]) == NULL ||
        aeon_tensor_cache_find(&cache, &tensors[1]) != NULL ||
        aeon_tensor_cache_find(&cache, &tensors[2]) == NULL ||
        aeon_tensor_cache_find(&cache, &tensors[3]) == NULL ||
        aeon_tensor_cache_find(&cache, &tensors[4]) == NULL) return 6;

    if (aeon_tensor_cache_access(&cache, &tensors[0]) == NULL ||
        aeon_tensor_cache_put(&cache, &materializations[1]) != 0 ||
        aeon_tensor_cache_find(&cache, &tensors[2]) != NULL) return 7;

    puts("AEON Batch 49 tensor cache recency test: PASS");
    return 0;
}
