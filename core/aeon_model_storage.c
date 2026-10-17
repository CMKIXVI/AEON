#include "aeon_model_storage.h"
#include <string.h>

int aeon_memory_model_read(void *context, size_t offset, void *buffer, size_t length)
{
    aeon_memory_model_storage_t *storage = (aeon_memory_model_storage_t *)context;
    if (!storage || !storage->data || !buffer || length == 0u) return -1;
    if (offset > storage->size || length > storage->size - offset) return -2;
    memcpy(buffer, storage->data + offset, length);
    return 0;
}

int aeon_model_storage_read(const aeon_memory_model_storage_t *storage,
                            aeon_model_stream_t *stream, void *buffer, size_t length)
{
    if (!storage || !stream) return -1;
    if (storage->size != stream->total_bytes) return -2;
    return aeon_model_stream_read(stream, aeon_memory_model_read,
                                  (void *)storage, buffer, length);
}
