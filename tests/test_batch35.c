#include "aeon_tensor_view.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned char storage[128];
    aeon_tensor_t tensor;
    aeon_tensor_view_t full;
    aeon_tensor_view_t slice;
    size_t shape[3] = {2u, 3u, 4u};
    size_t index[3] = {1u, 2u, 3u};
    size_t expected;

    memset(storage, 0, sizeof(storage));
    if (aeon_tensor_init(&tensor, AEON_TENSOR_I8, 3u, shape) != 0) return 1;
    tensor.data = storage;
    if (aeon_tensor_validate(&tensor) != 0) return 2;
    if (aeon_tensor_view_init(&full, &tensor) != 0) return 3;
    if (aeon_tensor_view_validate(&full, &tensor) != 0) return 4;

    expected = 1u * 12u + 2u * 4u + 3u;
    if (aeon_tensor_view_offset_elements(&full, index) != expected) return 5;

    if (aeon_tensor_view_slice(&full, 1u, 2u, &slice) != 0) return 6;
    if (slice.byte_offset != 8u || slice.shape[1] != 1u || slice.bytes != 8u) return 7;
    if (aeon_tensor_view_validate(&slice, &tensor) != 0) return 8;

    if (aeon_tensor_view_slice(&full, 1u, 3u, &slice) == 0) return 9;
    if (aeon_tensor_view_slice(&full, 3u, 0u, &slice) == 0) return 10;

    printf("AEON Batch 35 tensor view/slice test: PASS\n");
    return 0;
}
