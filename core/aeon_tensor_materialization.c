#include "aeon_tensor_materialization.h"

#include <stdint.h>

int aeon_tensor_materialization_validate(const aeon_tensor_materialization_t *materialization)
{
    if (materialization == NULL || materialization->buffer == NULL ||
        materialization->capacity == 0u || materialization->bytes == 0u ||
        materialization->bytes > materialization->capacity ||
        materialization->tensor == NULL) {
        return -1;
    }
    if (materialization->tensor->bytes != (uint64_t)materialization->bytes) {
        return -1;
    }
    return 0;
}

int aeon_tensor_materialization_init(aeon_tensor_materialization_t *materialization,
                                      void *buffer, size_t capacity)
{
    if (materialization == NULL || buffer == NULL || capacity == 0u) {
        return -1;
    }
    materialization->buffer = buffer;
    materialization->capacity = capacity;
    materialization->tensor = NULL;
    materialization->bytes = 0u;
    return 0;
}

int aeon_tensor_materialization_reset(aeon_tensor_materialization_t *materialization)
{
    if (materialization == NULL || materialization->buffer == NULL ||
        materialization->capacity == 0u) {
        return -1;
    }
    materialization->tensor = NULL;
    materialization->bytes = 0u;
    return 0;
}

int aeon_tensor_materialization_load(aeon_tensor_materialization_t *materialization,
                                     aeon_tensor_binding_t *binding)
{
    size_t bytes;

    if (materialization == NULL || binding == NULL ||
        materialization->buffer == NULL || materialization->capacity == 0u ||
        aeon_tensor_binding_validate(binding) != 0) {
        return -1;
    }
    bytes = binding->bytes;
    if (bytes == 0u || bytes > materialization->capacity || binding->tensor == NULL) {
        return -1;
    }
    if (aeon_tensor_binding_seek(binding, 0u) != 0 ||
        aeon_tensor_binding_read(binding, materialization->buffer, bytes) != 0) {
        return -1;
    }
    materialization->tensor = binding->tensor;
    materialization->bytes = bytes;
    return 0;
}
