#include <assert.h>
#include <stdio.h>
#include "aeon_tensor_cache_invalidation.h"

int main(void)
{
    aeon_tensor_cache_t cache;
    aeon_tensor_materialization_t materialization;
    aeon_model_tensor_meta_t tensor = {0};
    unsigned char buffer[16] = {0};

    tensor.bytes = sizeof(buffer);
    assert(aeon_tensor_cache_init(&cache) == 0);
    assert(aeon_tensor_materialization_init(&materialization, buffer, sizeof(buffer)) == 0);
    materialization.tensor = &tensor;
    materialization.bytes = sizeof(buffer);
    assert(aeon_tensor_cache_put(&cache, &materialization) == 0);
    assert(aeon_tensor_cache_find(&cache, &tensor) != NULL);
    assert(aeon_tensor_cache_invalidate(&cache, &tensor) == 0);
    assert(aeon_tensor_cache_find(&cache, &tensor) == NULL);
    assert(aeon_tensor_cache_invalidate(&cache, &tensor) == 1);
    assert(aeon_tensor_cache_invalidate_all(&cache) == 0);
    puts("AEON Batch 48 tensor cache invalidation test: PASS");
    return 0;
}
