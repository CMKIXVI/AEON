#include "aeon_model_index.h"
#include "aeon_model_stream.h"
#include "aeon_tensor_binding.h"

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
    uint8_t out[8];
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_binding_t binding;
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) {
        model[i] = (uint8_t)i;
    }
    if (aeon_model_index_init(&index) != 0 ||
        aeon_model_index_add(&index, "weight", 16u, 32u,
                             AEON_MODEL_DTYPE_F32, 2u, shape) != 0 ||
        aeon_model_index_validate(&index, sizeof(model)) != 0 ||
        aeon_model_stream_init(&stream, sizeof(model)) != 0 ||
        aeon_model_stream_bind(&stream, fake_read, model) != 0 ||
        aeon_tensor_binding_init(&binding, &stream, &index, "weight") != 0) {
        return 1;
    }
    if (aeon_tensor_binding_remaining(&binding) != 32u ||
        aeon_tensor_binding_read(&binding, out, sizeof(out)) != 0 ||
        aeon_tensor_binding_remaining(&binding) != 24u ||
        memcmp(out, model + 16u, sizeof(out)) != 0) {
        return 2;
    }
    if (aeon_tensor_binding_seek(&binding, 28u) != 0 ||
        aeon_tensor_binding_read(&binding, out, 5u) == 0) {
        return 3;
    }
    if (aeon_tensor_binding_seek(&binding, 28u) != 0 ||
        aeon_tensor_binding_read(&binding, out, 4u) != 0 ||
        memcmp(out, model + 44u, 4u) != 0) {
        return 4;
    }
    if (aeon_tensor_binding_init(&binding, &stream, &index, "missing") == 0) {
        return 5;
    }

    puts("AEON Batch 45 tensor binding test: PASS");
    return 0;
}
