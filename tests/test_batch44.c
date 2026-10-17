#include "aeon_tensor_layout.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    aeon_model_tensor_meta_t tensor = {0};
    uint64_t elements = 0u;
    uint64_t bytes = 0u;
    size_t element_size = 0u;

    tensor.dtype = AEON_MODEL_DTYPE_F32;
    tensor.rank = 2u;
    tensor.shape[0] = 2u;
    tensor.shape[1] = 4u;
    tensor.bytes = 32u;

    assert(aeon_model_dtype_size(AEON_MODEL_DTYPE_F32, &element_size) == 0);
    assert(element_size == 4u);
    assert(aeon_tensor_element_count(&tensor, &elements) == 0);
    assert(elements == 8u);
    assert(aeon_tensor_expected_bytes(&tensor, &bytes) == 0);
    assert(bytes == 32u);
    assert(aeon_tensor_layout_validate(&tensor) == 0);

    tensor.bytes = 16u;
    assert(aeon_tensor_layout_validate(&tensor) != 0);
    tensor.bytes = 32u;
    tensor.shape[1] = UINT64_MAX;
    assert(aeon_tensor_element_count(&tensor, &elements) != 0);

    puts("AEON Batch 44 tensor layout test: PASS");
    return 0;
}
