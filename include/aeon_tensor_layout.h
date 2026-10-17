#ifndef AEON_TENSOR_LAYOUT_H
#define AEON_TENSOR_LAYOUT_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_index.h"

int aeon_model_dtype_size(aeon_model_dtype_t dtype, size_t *bytes_per_element);
int aeon_tensor_element_count(const aeon_model_tensor_meta_t *tensor, uint64_t *elements);
int aeon_tensor_expected_bytes(const aeon_model_tensor_meta_t *tensor, uint64_t *bytes);
int aeon_tensor_layout_validate(const aeon_model_tensor_meta_t *tensor);

#endif
