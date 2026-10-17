#include "aeon_tensor_cache_materialization.h"

#include <string.h>

int aeon_tensor_cache_materialize(aeon_tensor_cache_t *cache,
                                  aeon_tensor_materialization_t *materialization,
                                  aeon_tensor_binding_t *binding)
{
    const aeon_tensor_cache_entry_t *entry;

    if (cache == NULL || materialization == NULL || binding == NULL ||
        aeon_tensor_binding_validate(binding) != 0 ||
        materialization->buffer == NULL || materialization->capacity == 0u) {
        return -1;
    }
    if (binding->bytes > materialization->capacity || binding->bytes == 0u) {
        return -1;
    }

    entry = aeon_tensor_cache_access(cache, binding->tensor);
    if (entry != NULL) {
        if (entry->materialization == NULL || entry->bytes != binding->bytes ||
            aeon_tensor_materialization_validate(entry->materialization) != 0 ||
            entry->materialization->tensor != binding->tensor) {
            (void)aeon_tensor_cache_detach(cache, entry->materialization);
            entry = NULL;
        } else {
            memcpy(materialization->buffer, entry->materialization->buffer, entry->bytes);
            materialization->tensor = binding->tensor;
            materialization->bytes = entry->bytes;
            return aeon_tensor_materialization_validate(materialization);
        }
    }

    if (aeon_tensor_materialization_load(materialization, binding) != 0 ||
        aeon_tensor_materialization_validate(materialization) != 0) {
        return -1;
    }
    return aeon_tensor_cache_put(cache, materialization);
}
