#ifndef AEON_TENSOR_VIEW_H
#define AEON_TENSOR_VIEW_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_tensor.h"

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint8_t rank;
    uint8_t reserved;
    aeon_tensor_dtype_t dtype;
    size_t shape[AEON_TENSOR_MAX_DIMS];
    size_t stride[AEON_TENSOR_MAX_DIMS];
    size_t elements;
    size_t bytes;
    size_t byte_offset;
    const void *data;
} aeon_tensor_view_t;

#define AEON_TENSOR_VIEW_MAGIC 0x41455631u
#define AEON_TENSOR_VIEW_VERSION 1u

int aeon_tensor_view_init(aeon_tensor_view_t *view, const aeon_tensor_t *tensor);
int aeon_tensor_view_slice(const aeon_tensor_view_t *source, uint8_t axis,
                           size_t index, aeon_tensor_view_t *view);
int aeon_tensor_view_validate(const aeon_tensor_view_t *view,
                              const aeon_tensor_t *owner);
size_t aeon_tensor_view_offset_elements(const aeon_tensor_view_t *view,
                                        const size_t *indices);

#endif
