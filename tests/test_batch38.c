#include "aeon_nvme_storage.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

static uint8_t disk[32];

static int fake_read(void *context, uint64_t lba, size_t count, void *buffer)
{
    size_t offset = (size_t)lba * 4u;
    size_t bytes = count * 4u;
    (void)context;
    if (offset + bytes > sizeof(disk)) return -1;
    memcpy(buffer, disk + offset, bytes);
    return 0;
}

int main(void)
{
    aeon_nvme_storage_t storage;
    aeon_model_stream_t stream;
    uint8_t output[8];
    size_t i;

    for (i = 0u; i < sizeof(disk); ++i) disk[i] = (uint8_t)(i + 1u);
    assert(aeon_nvme_storage_init(&storage, sizeof(disk), 4u, fake_read, NULL) == 0);
    assert(aeon_nvme_storage_bind_stream(&stream, &storage) == 0);
    assert(aeon_model_stream_seek(&stream, 8u) == 0);
    assert(aeon_model_stream_read(&stream, aeon_nvme_storage_read,
                                  &storage, output, sizeof(output)) == 0);
    assert(memcmp(output, disk + 8u, sizeof(output)) == 0);
    assert(aeon_model_stream_seek(&stream, 6u) != 0);
    assert(aeon_nvme_storage_read(&storage, 28u, output, 8u) != 0);
    return 0;
}
