#include "aeon_model_manifest.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int read_memory(void *context, size_t offset, void *buffer, size_t length)
{
    unsigned char *memory = (unsigned char *)context;
    memcpy(buffer, memory + offset, length);
    return 0;
}

int main(void)
{
    unsigned char model[64];
    unsigned char output[32];
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    const char manifest[] =
        "# AEON model manifest\n"
        "layer.0.weight|0|32|F32|2|2,4\n"
        "layer.1.weight|32|32|BF16|2|4,4\n";
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) model[i] = (unsigned char)i;
    assert(aeon_model_manifest_parse(&index, manifest, strlen(manifest), 64u) == 0);
    assert(index.count == 2u);
    assert(aeon_model_stream_init(&stream, sizeof(model)) == 0);
    assert(aeon_model_stream_bind(&stream, read_memory, model) == 0);
    assert(aeon_model_manifest_load_tensor(&stream, &index, "layer.1.weight",
                                           output, sizeof(output)) == 0);
    assert(output[0] == 32u && output[31] == 63u);
    assert(aeon_model_manifest_load_tensor(&stream, &index, "layer.1.weight",
                                           output, 4u) != 0);
    assert(aeon_model_manifest_parse(&index, "bad|60|8|F32|1|8\n", 17u, 64u) != 0);
    puts("AEON Batch 43 model manifest/loader test: PASS");
    return 0;
}
