#include "aeon_model_index.h"
#include "aeon_model_stream.h"
#include "aeon_tensor_binding.h"
#include "aeon_tensor_materialization.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int fake_read(void *context, size_t offset, void *buffer, size_t length)
{
    const uint8_t *data = (const uint8_t *)context;
    memcpy(buffer, data + offset, length);
    return 0;
}

int main(void)
{
    uint8_t model[64];
    uint64_t shape[2] = {2u, 4u};
    uint8_t out[32];
    uint8_t small[16];
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_binding_t binding;
    aeon_tensor_materialization_t materialization;
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) {
        model[i] = (uint8_t)(0xA0u + i);
    }
    if (aeon_model_index_init(&index) != 0 ||
        aeon_model_index_add(&index, "weight", 16u, 32u,
                             AEON_MODEL_DTYPE_F32, 2u, shape) != 0 ||
        aeon_model_index_validate(&index, sizeof(model)) != 0 ||
        aeon_model_stream_init(&stream, sizeof(model)) != 0 ||
        aeon_model_stream_bind(&stream, fake_read, model) != 0 ||
        aeon_tensor_binding_init(&binding, &stream, &index, "weight") != 0 ||
        aeon_tensor_materialization_init(&materialization, out, sizeof(out)) != 0) {
        return 1;
    }
    if (aeon_tensor_materialization_load(&materialization, &binding) != 0 ||
        aeon_tensor_materialization_validate(&materialization) != 0 ||
        materialization.bytes != 32u || materialization.tensor != binding.tensor ||
        memcmp(out, model + 16u, 32u) != 0) {
        return 2;
    }
    if (aeon_tensor_materialization_reset(&materialization) != 0 ||
        aeon_tensor_materialization_validate(&materialization) == 0) {
        return 3;
    }
    if (aeon_tensor_materialization_init(&materialization, small, sizeof(small)) != 0 ||
        aeon_tensor_materialization_load(&materialization, &binding) == 0) {
        return 4;
    }

    puts("AEON Batch 46 tensor materialization test: PASS");
    return 0;
}
