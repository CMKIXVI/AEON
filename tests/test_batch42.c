#include "aeon_model_index.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    aeon_model_index_t index;
    uint64_t shape[2] = {2u, 4u};

    assert(aeon_model_index_init(&index) == 0);
    assert(aeon_model_index_add(&index, "layer.0.weight", 0u, 32u,
                                AEON_MODEL_DTYPE_F32, 2u, shape) == 0);
    assert(aeon_model_index_add(&index, "layer.1.weight", 32u, 32u,
                                AEON_MODEL_DTYPE_F32, 2u, shape) == 0);
    assert(aeon_model_index_add(&index, "layer.1.weight", 64u, 32u,
                                AEON_MODEL_DTYPE_F32, 2u, shape) == -2);
    assert(aeon_model_index_validate(&index, 64u) == 0);
    assert(aeon_model_index_find(&index, "layer.0.weight") != NULL);
    assert(aeon_model_index_find(&index, "layer.0.weight")->bytes == 32u);

    index.tensors[1].offset = 16u;
    assert(aeon_model_index_validate(&index, 64u) == -5);

    puts("AEON Batch 42 model index test: PASS");
    return 0;
}
