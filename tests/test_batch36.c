#include "aeon_model_stream.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

static int reader(void *context, size_t offset, void *buffer, size_t length)
{
    const uint8_t *source = (const uint8_t *)context;
    memcpy(buffer, source + offset, length);
    return 0;
}

int main(void)
{
    uint8_t source[32];
    uint8_t output[8];
    aeon_model_stream_t stream;
    size_t i;
    for (i = 0u; i < sizeof(source); ++i) source[i] = (uint8_t)i;
    assert(aeon_model_stream_init(&stream, sizeof(source)) == 0);
    assert(aeon_model_stream_read(&stream, reader, source, output, sizeof(output)) == 0);
    assert(memcmp(output, source, sizeof(output)) == 0);
    assert(stream.position == sizeof(output));
    assert(aeon_model_stream_seek(&stream, 16u) == 0);
    assert(aeon_model_stream_remaining(&stream) == 16u);
    assert(aeon_model_stream_read(&stream, reader, source, output, sizeof(output)) == 0);
    assert(memcmp(output, source + 16u, sizeof(output)) == 0);
    assert(aeon_model_stream_read(&stream, reader, source, output, 9u) == 0);
    assert(aeon_model_stream_read(&stream, reader, source, output, 1u) != 0);
    stream.position = stream.total_bytes + 1u;
    assert(aeon_model_stream_validate(&stream) != 0);
    return 0;
}
