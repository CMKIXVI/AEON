#ifndef AEON_TENSOR_MATERIALIZATION_H
#define AEON_TENSOR_MATERIALIZATION_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_tensor_binding.h"

typedef struct {
    void *buffer;
    size_t capacity;
    const aeon_model_tensor_meta_t *tensor;
    size_t bytes;
} aeon_tensor_materialization_t;

int aeon_tensor_materialization_init(aeon_tensor_materialization_t *materialization,
                                      void *buffer, size_t capacity);
int aeon_tensor_materialization_reset(aeon_tensor_materialization_t *materialization);
int aeon_tensor_materialization_load(aeon_tensor_materialization_t *materialization,
                                     aeon_tensor_binding_t *binding);
int aeon_tensor_materialization_validate(const aeon_tensor_materialization_t *materialization);

#endif
