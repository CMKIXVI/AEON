#include "aeon_tensor_cache.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint8_t data[32] = {0};
    aeon_model_tensor_meta_t tensor = {0};
    aeon_tensor_materialization_t materialization;
    aeon_tensor_cache_t cache;

    tensor.bytes = sizeof(data);
    if (aeon_tensor_materialization_init(&materialization, data, sizeof(data)) != 0) return 1;
    materialization.tensor = &tensor;
    materialization.bytes = sizeof(data);
    if (aeon_tensor_materialization_validate(&materialization) != 0) return 2;
    if (aeon_tensor_cache_init(&cache) != 0) return 3;
    if (aeon_tensor_cache_find(&cache, &tensor) != NULL) return 4;
    if (aeon_tensor_cache_put(&cache, &materialization) != 0) return 5;
    if (aeon_tensor_cache_find(&cache, &tensor) == NULL) return 6;
    if (aeon_tensor_cache_put(&cache, &materialization) != 0) return 7;
    if (aeon_tensor_cache_reset(&cache) != 0 ||
        aeon_tensor_cache_find(&cache, &tensor) != NULL) return 8;

    puts("AEON Batch 47 tensor cache test: PASS");
    return 0;
}
