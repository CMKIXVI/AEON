#include "aeon_model_storage.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
    uint8_t model[64];
    uint8_t out[16];
    aeon_memory_model_storage_t storage;
    aeon_model_stream_t stream;
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) model[i] = (uint8_t)(i ^ 0x5Au);
    storage.data = model;
    storage.size = sizeof(model);

    assert(aeon_model_stream_init(&stream, sizeof(model)) == 0);
    assert(aeon_model_storage_read(&storage, &stream, out, sizeof(out)) == 0);
    assert(memcmp(out, model, sizeof(out)) == 0);
    assert(aeon_model_stream_seek(&stream, 32u) == 0);
    assert(aeon_model_storage_read(&storage, &stream, out, sizeof(out)) == 0);
    assert(memcmp(out, model + 32u, sizeof(out)) == 0);
    assert(aeon_model_storage_read(&storage, &stream, out, 17u) != 0);
    storage.size = 63u;
    assert(aeon_model_storage_read(&storage, &stream, out, 1u) != 0);
    return 0;
}
