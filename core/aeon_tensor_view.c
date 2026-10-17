#include "aeon_tensor_view.h"
#include <stdint.h>
#include <string.h>

static int checked_add(size_t a, size_t b, size_t *out)
{
    if (!out || b > SIZE_MAX - a) return -1;
    *out = a + b;
    return 0;
}

static int checked_mul(size_t a, size_t b, size_t *out)
{
    if (!out || (a != 0u && b > SIZE_MAX / a)) return -1;
    *out = a * b;
    return 0;
}

static int view_shape_elements(const aeon_tensor_view_t *view, size_t *elements)
{
    size_t n = 1u;
    uint8_t i;
    if (!view || !elements || view->rank == 0u || view->rank > AEON_TENSOR_MAX_DIMS) return -1;
    for (i = 0u; i < view->rank; ++i) {
        if (view->shape[i] == 0u || checked_mul(n, view->shape[i], &n) != 0) return -1;
    }
    *elements = n;
    return 0;
}

int aeon_tensor_view_init(aeon_tensor_view_t *view, const aeon_tensor_t *tensor)
{
    if (!view || !tensor || aeon_tensor_validate(tensor) != 0) return -1;
    memset(view, 0, sizeof(*view));
    view->magic = AEON_TENSOR_VIEW_MAGIC;
    view->version = AEON_TENSOR_VIEW_VERSION;
    view->rank = tensor->rank;
    view->dtype = tensor->dtype;
    memcpy(view->shape, tensor->shape, sizeof(view->shape));
    memcpy(view->stride, tensor->stride, sizeof(view->stride));
    view->elements = tensor->elements;
    view->bytes = tensor->bytes;
    view->data = tensor->data;
    return 0;
}

int aeon_tensor_view_slice(const aeon_tensor_view_t *source, uint8_t axis,
                           size_t index, aeon_tensor_view_t *view)
{
    size_t element_offset;
    size_t byte_offset;
    size_t item_size;
    if (!source || !view || source->magic != AEON_TENSOR_VIEW_MAGIC ||
        source->version != AEON_TENSOR_VIEW_VERSION || axis >= source->rank ||
        index >= source->shape[axis] || !source->data) return -1;
    item_size = aeon_tensor_dtype_size(source->dtype);
    if (item_size == 0u || checked_mul(index, source->stride[axis], &element_offset) != 0 ||
        checked_mul(element_offset, item_size, &byte_offset) != 0 ||
        checked_add(source->byte_offset, byte_offset, &byte_offset) != 0) return -2;
    *view = *source;
    view->byte_offset = byte_offset;
    view->data = (const unsigned char *)source->data + byte_offset;
    view->shape[axis] = 1u;
    if (view_shape_elements(view, &view->elements) != 0 ||
        checked_mul(view->elements, item_size, &view->bytes) != 0) return -3;
    return 0;
}

int aeon_tensor_view_validate(const aeon_tensor_view_t *view,
                              const aeon_tensor_t *owner)
{
    size_t elements;
    size_t span_elements = 1u;
    size_t axis_span;
    size_t span_bytes;
    size_t item_size;
    uint8_t i;
    if (!view || !owner || aeon_tensor_validate(owner) != 0 ||
        view->magic != AEON_TENSOR_VIEW_MAGIC ||
        view->version != AEON_TENSOR_VIEW_VERSION || view->rank == 0u ||
        view->rank > AEON_TENSOR_MAX_DIMS || view->dtype != owner->dtype ||
        !view->data) return -1;
    item_size = aeon_tensor_dtype_size(view->dtype);
    if (item_size == 0u || view_shape_elements(view, &elements) != 0 ||
        elements != view->elements || checked_mul(elements, item_size, &span_bytes) != 0 ||
        view->bytes != span_bytes) return -2;
    for (i = 0u; i < view->rank; ++i) {
        if (view->shape[i] > 1u &&
            checked_mul(view->shape[i] - 1u, view->stride[i], &axis_span) != 0) return -3;
        if (view->shape[i] > 1u &&
            checked_add(span_elements, axis_span, &span_elements) != 0) return -4;
    }
    if (checked_mul(span_elements, item_size, &span_bytes) != 0 ||
        view->byte_offset > owner->bytes || span_bytes > owner->bytes - view->byte_offset) return -5;
    return 0;
}

size_t aeon_tensor_view_offset_elements(const aeon_tensor_view_t *view,
                                        const size_t *indices)
{
    size_t offset = 0u;
    size_t term;
    uint8_t i;
    if (!view || !indices || view->magic != AEON_TENSOR_VIEW_MAGIC ||
        view->version != AEON_TENSOR_VIEW_VERSION) return SIZE_MAX;
    for (i = 0u; i < view->rank; ++i) {
        if (indices[i] >= view->shape[i] ||
            checked_mul(indices[i], view->stride[i], &term) != 0 ||
            checked_add(offset, term, &offset) != 0) return SIZE_MAX;
    }
    return offset;
}
