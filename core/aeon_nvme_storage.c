#include "aeon_nvme_storage.h"
#include <stdint.h>

static int checked_add_size(size_t a, size_t b, size_t *out)
{
    if (!out || b > SIZE_MAX - a) return -1;
    *out = a + b;
    return 0;
}

int aeon_nvme_storage_init(aeon_nvme_storage_t *storage,
                           uint64_t capacity_bytes, uint32_t lba_bytes,
                           aeon_nvme_read_lba_fn read_lba, void *context)
{
    if (!storage || capacity_bytes == 0u || lba_bytes == 0u ||
        (lba_bytes & (lba_bytes - 1u)) != 0u || !read_lba) return -1;
    if (capacity_bytes % lba_bytes != 0u) return -1;
    storage->capacity_bytes = capacity_bytes;
    storage->lba_bytes = lba_bytes;
    storage->read_lba = read_lba;
    storage->context = context;
    return 0;
}

int aeon_nvme_storage_read(void *context, size_t offset, void *buffer, size_t length)
{
    aeon_nvme_storage_t *storage = (aeon_nvme_storage_t *)context;
    size_t end;
    uint64_t lba;
    size_t lba_count;

    if (!storage || !buffer || length == 0u || storage->lba_bytes == 0u ||
        !storage->read_lba) return -1;
    if (checked_add_size(offset, length, &end) != 0) return -1;
    if ((uint64_t)end > storage->capacity_bytes) return -2;
    if ((offset % storage->lba_bytes) != 0u ||
        (length % storage->lba_bytes) != 0u) return -3;

    lba = (uint64_t)(offset / storage->lba_bytes);
    lba_count = length / storage->lba_bytes;
    return storage->read_lba(storage->context, lba, lba_count, buffer);
}

int aeon_nvme_storage_bind_stream(aeon_model_stream_t *stream,
                                  aeon_nvme_storage_t *storage)
{
    if (!stream || !storage || storage->capacity_bytes == 0u ||
        storage->capacity_bytes > SIZE_MAX) return -1;
    return aeon_model_stream_init(stream, (size_t)storage->capacity_bytes);
}
