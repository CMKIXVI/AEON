#ifndef AEON_TENSOR_H
#define AEON_TENSOR_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_runtime_memory.h"

#define AEON_TENSOR_MAGIC 0x41455431u
#define AEON_TENSOR_VERSION 1u
#define AEON_TENSOR_MAX_DIMS 8u

typedef enum {
    AEON_TENSOR_F32 = 1,
    AEON_TENSOR_BF16 = 2,
    AEON_TENSOR_F16 = 3,
    AEON_TENSOR_I8 = 4,
    AEON_TENSOR_I16 = 5,
    AEON_TENSOR_I32 = 6
} aeon_tensor_dtype_t;

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    aeon_tensor_dtype_t dtype;
    uint8_t rank;
    uint8_t reserved[3];
    size_t shape[AEON_TENSOR_MAX_DIMS];
    size_t stride[AEON_TENSOR_MAX_DIMS];
    size_t elements;
    size_t bytes;
    void *data;
} aeon_tensor_t;

size_t aeon_tensor_dtype_size(aeon_tensor_dtype_t dtype);
int aeon_tensor_init(aeon_tensor_t *tensor, aeon_tensor_dtype_t dtype,
                     uint8_t rank, const size_t *shape);
int aeon_tensor_bind(aeon_tensor_t *tensor, aeon_runtime_memory_t *memory);
int aeon_tensor_validate(const aeon_tensor_t *tensor);
int aeon_tensor_zero(aeon_tensor_t *tensor);

#endif
