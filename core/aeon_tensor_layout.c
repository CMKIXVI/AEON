#include "aeon_tensor_layout.h"

#include <limits.h>

int aeon_model_dtype_size(aeon_model_dtype_t dtype, size_t *bytes_per_element)
{
    if (bytes_per_element == NULL) return -1;
    switch (dtype) {
    case AEON_MODEL_DTYPE_F32:
        *bytes_per_element = 4u;
        return 0;
    case AEON_MODEL_DTYPE_BF16:
    case AEON_MODEL_DTYPE_F16:
        *bytes_per_element = 2u;
        return 0;
    case AEON_MODEL_DTYPE_FP8_E4M3:
    case AEON_MODEL_DTYPE_I8:
        *bytes_per_element = 1u;
        return 0;
    default:
        return -1;
    }
}

int aeon_tensor_element_count(const aeon_model_tensor_meta_t *tensor, uint64_t *elements)
{
    uint64_t count = 1u;
    size_t i;

    if (tensor == NULL || elements == NULL || tensor->rank == 0u ||
        tensor->rank > AEON_MODEL_INDEX_MAX_DIMS) return -1;
    for (i = 0u; i < (size_t)tensor->rank; ++i) {
        if (tensor->shape[i] == 0u || count > UINT64_MAX / tensor->shape[i]) return -1;
        count *= tensor->shape[i];
    }
    *elements = count;
    return 0;
}

int aeon_tensor_expected_bytes(const aeon_model_tensor_meta_t *tensor, uint64_t *bytes)
{
    uint64_t elements;
    size_t element_size;

    if (bytes == NULL || aeon_tensor_element_count(tensor, &elements) != 0 ||
        aeon_model_dtype_size(tensor->dtype, &element_size) != 0 ||
        elements > UINT64_MAX / (uint64_t)element_size) return -1;
    *bytes = elements * (uint64_t)element_size;
    return 0;
}

int aeon_tensor_layout_validate(const aeon_model_tensor_meta_t *tensor)
{
    uint64_t expected;

    if (tensor == NULL || tensor->bytes == 0u ||
        aeon_tensor_expected_bytes(tensor, &expected) != 0 ||
        tensor->bytes != expected) return -1;
    return 0;
}
