#ifndef AEON_MODEL_STORAGE_H
#define AEON_MODEL_STORAGE_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_stream.h"

typedef struct {
    const uint8_t *data;
    size_t size;
} aeon_memory_model_storage_t;

int aeon_memory_model_read(void *context, size_t offset, void *buffer, size_t length);
int aeon_model_storage_read(const aeon_memory_model_storage_t *storage,
                            aeon_model_stream_t *stream, void *buffer, size_t length);

#endif
