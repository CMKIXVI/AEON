#include "aeon_tensor.h"
#include <stdint.h>
#include <string.h>

static int checked_mul(size_t a, size_t b, size_t *out)
{
    if (!out || (a != 0 && b > SIZE_MAX / a)) return -1;
    *out = a * b;
    return 0;
}

size_t aeon_tensor_dtype_size(aeon_tensor_dtype_t dtype)
{
    switch (dtype) {
    case AEON_TENSOR_F32: return 4u;
    case AEON_TENSOR_BF16: return 2u;
    case AEON_TENSOR_F16: return 2u;
    case AEON_TENSOR_I8: return 1u;
    case AEON_TENSOR_I16: return 2u;
    case AEON_TENSOR_I32: return 4u;
    default: return 0u;
    }
}

int aeon_tensor_init(aeon_tensor_t *tensor, aeon_tensor_dtype_t dtype,
                     uint8_t rank, const size_t *shape)
{
    size_t elements = 1u;
    size_t item_size;
    uint8_t i;
    if (!tensor || !shape || rank == 0u || rank > AEON_TENSOR_MAX_DIMS) return -1;
    item_size = aeon_tensor_dtype_size(dtype);
    if (item_size == 0u) return -2;
    memset(tensor, 0, sizeof(*tensor));
    for (i = 0u; i < rank; ++i) {
        if (shape[i] == 0u || checked_mul(elements, shape[i], &elements) != 0) return -3;
        tensor->shape[i] = shape[i];
    }
    tensor->magic = AEON_TENSOR_MAGIC;
    tensor->version = AEON_TENSOR_VERSION;
    tensor->dtype = dtype;
    tensor->rank = rank;
    tensor->elements = elements;
    if (checked_mul(elements, item_size, &tensor->bytes) != 0) return -4;
    tensor->stride[rank - 1u] = 1u;
    for (i = rank - 1u; i > 0u; --i) {
        if (checked_mul(tensor->stride[i], tensor->shape[i], &tensor->stride[i - 1u]) != 0) return -5;
    }
    return 0;
}

int aeon_tensor_validate(const aeon_tensor_t *tensor)
{
    size_t item_size;
    size_t elements = 1u;
    uint8_t i;
    if (!tensor || tensor->magic != AEON_TENSOR_MAGIC ||
        tensor->version != AEON_TENSOR_VERSION || tensor->rank == 0u ||
        tensor->rank > AEON_TENSOR_MAX_DIMS || !tensor->data) return -1;
    item_size = aeon_tensor_dtype_size(tensor->dtype);
    if (item_size == 0u) return -2;
    for (i = 0u; i < tensor->rank; ++i) {
        if (tensor->shape[i] == 0u || checked_mul(elements, tensor->shape[i], &elements) != 0) return -3;
    }
    if (elements != tensor->elements) return -4;
    if (elements > SIZE_MAX / item_size || elements * item_size != tensor->bytes) return -5;
    return 0;
}

int aeon_tensor_bind(aeon_tensor_t *tensor, aeon_runtime_memory_t *memory)
{
    if (!tensor || !memory || tensor->magic != AEON_TENSOR_MAGIC ||
        tensor->version != AEON_TENSOR_VERSION || tensor->bytes == 0u) return -1;
    tensor->data = aeon_runtime_memory_alloc(memory, tensor->bytes);
    return tensor->data ? 0 : -2;
}

int aeon_tensor_zero(aeon_tensor_t *tensor)
{
    if (aeon_tensor_validate(tensor) != 0) return -1;
    memset(tensor->data, 0, tensor->bytes);
    return 0;
}
