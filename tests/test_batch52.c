#include "aeon_tensor_cache.h"

#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t data[8] = {0};
    uint64_t shape[1] = {2u};
    aeon_model_tensor_meta_t tensor = {0};
    aeon_tensor_materialization_t materialization;
    aeon_tensor_cache_t cache;

    tensor.dtype = AEON_MODEL_DTYPE_F32;
    tensor.rank = 1u;
    tensor.shape[0] = shape[0];
    tensor.bytes = sizeof(data);

    if (aeon_tensor_materialization_init(&materialization, data, sizeof(data)) != 0) return 1;
    materialization.tensor = &tensor;
    materialization.bytes = sizeof(data);
    if (aeon_tensor_materialization_validate(&materialization) != 0) return 2;
    if (aeon_tensor_cache_init(&cache) != 0) return 3;
    if (aeon_tensor_cache_put(&cache, &materialization) != 0) return 4;
    if (aeon_tensor_cache_find(&cache, &tensor) == NULL) return 5;

    if (aeon_tensor_cache_detach(&cache, &materialization) != 0) return 6;
    if (aeon_tensor_cache_find(&cache, &tensor) != NULL) return 7;
    if (aeon_tensor_cache_detach(&cache, &materialization) != 1) return 8;

    if (aeon_tensor_materialization_reset(&materialization) != 0) return 9;
    puts("AEON Batch 52 tensor cache lifetime test: PASS");
    return 0;
}
