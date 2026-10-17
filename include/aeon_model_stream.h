#ifndef AEON_MODEL_STREAM_H
#define AEON_MODEL_STREAM_H

#include <stddef.h>
#include <stdint.h>

#define AEON_MODEL_STREAM_MAGIC 0x41454D53u
#define AEON_MODEL_STREAM_VERSION 2u

typedef int (*aeon_model_read_fn)(void *context, size_t offset, void *buffer, size_t length);

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    size_t total_bytes;
    size_t position;
    aeon_model_read_fn reader;
    void *context;
} aeon_model_stream_t;

int aeon_model_stream_init(aeon_model_stream_t *stream, size_t total_bytes);
int aeon_model_stream_validate(const aeon_model_stream_t *stream);
int aeon_model_stream_bind(aeon_model_stream_t *stream, aeon_model_read_fn reader,
                           void *context);
int aeon_model_stream_seek(aeon_model_stream_t *stream, size_t offset);
int aeon_model_stream_read(aeon_model_stream_t *stream, aeon_model_read_fn reader,
                           void *context, void *buffer, size_t length);
int aeon_model_stream_read_bound(aeon_model_stream_t *stream, void *buffer, size_t length);
size_t aeon_model_stream_remaining(const aeon_model_stream_t *stream);

#endif
