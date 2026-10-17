#include "aeon_model_stream.h"

#include <stdint.h>

int aeon_model_stream_init(aeon_model_stream_t *stream, size_t total_bytes)
{
    if (stream == NULL || total_bytes == 0u) {
        return -1;
    }
    stream->magic = AEON_MODEL_STREAM_MAGIC;
    stream->version = AEON_MODEL_STREAM_VERSION;
    stream->flags = 0u;
    stream->total_bytes = total_bytes;
    stream->position = 0u;
    stream->reader = NULL;
    stream->context = NULL;
    return 0;
}

int aeon_model_stream_validate(const aeon_model_stream_t *stream)
{
    if (stream == NULL || stream->magic != AEON_MODEL_STREAM_MAGIC ||
        stream->version != AEON_MODEL_STREAM_VERSION || stream->total_bytes == 0u ||
        stream->position > stream->total_bytes) {
        return -1;
    }
    return 0;
}

int aeon_model_stream_bind(aeon_model_stream_t *stream, aeon_model_read_fn reader,
                           void *context)
{
    if (aeon_model_stream_validate(stream) != 0 || reader == NULL) {
        return -1;
    }
    stream->reader = reader;
    stream->context = context;
    return 0;
}

int aeon_model_stream_seek(aeon_model_stream_t *stream, size_t offset)
{
    if (aeon_model_stream_validate(stream) != 0 || offset > stream->total_bytes) {
        return -1;
    }
    stream->position = offset;
    return 0;
}

int aeon_model_stream_read(aeon_model_stream_t *stream, aeon_model_read_fn reader,
                           void *context, void *buffer, size_t length)
{
    if (aeon_model_stream_validate(stream) != 0 || reader == NULL ||
        buffer == NULL || length == 0u) {
        return -1;
    }
    if (length > stream->total_bytes - stream->position) {
        return -2;
    }
    if (reader(context, stream->position, buffer, length) != 0) {
        return -3;
    }
    stream->position += length;
    return 0;
}

int aeon_model_stream_read_bound(aeon_model_stream_t *stream, void *buffer, size_t length)
{
    if (aeon_model_stream_validate(stream) != 0 || stream->reader == NULL) {
        return -1;
    }
    return aeon_model_stream_read(stream, stream->reader, stream->context, buffer, length);
}

size_t aeon_model_stream_remaining(const aeon_model_stream_t *stream)
{
    if (aeon_model_stream_validate(stream) != 0) {
        return 0u;
    }
    return stream->total_bytes - stream->position;
}
