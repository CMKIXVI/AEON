#include <stdio.h>
#include <string.h>
#include "aeon_model_stream.h"
#include "aeon_nvme_queue_read.h"

typedef struct {
    unsigned char disk[32];
    uint64_t lba;
    uint32_t count;
    uint16_t cid;
    int completed;
    int success;
} fake_nvme_t;

static int submit(void *context, uint64_t lba, uint32_t count,
                  void *buffer, size_t bytes, uint16_t *cid)
{
    fake_nvme_t *d = (fake_nvme_t *)context;
    size_t offset = (size_t)lba * 4u;
    if (offset + bytes > sizeof(d->disk) || count == 0u || bytes != (size_t)count * 4u) {
        return -1;
    }
    memcpy(buffer, d->disk + offset, bytes);
    d->lba = lba;
    d->count = count;
    d->cid = 7u;
    d->completed = 1;
    d->success = 1;
    *cid = d->cid;
    return 0;
}

static int poll(void *context, uint16_t cid, int *completed, int *success)
{
    fake_nvme_t *d = (fake_nvme_t *)context;
    if (cid != d->cid) {
        return -1;
    }
    *completed = d->completed;
    *success = d->success;
    return 0;
}

int main(void)
{
    fake_nvme_t fake;
    aeon_nvme_queue_read_t reader;
    aeon_model_stream_t stream;
    unsigned char out[8];
    size_t i;

    for (i = 0u; i < sizeof(fake.disk); ++i) {
        fake.disk[i] = (unsigned char)i;
    }
    fake.completed = 0;
    fake.success = 0;

    if (aeon_nvme_queue_read_init(&reader, sizeof(fake.disk), 4u,
                                  submit, poll, &fake) != 0) {
        return 1;
    }
    if (aeon_nvme_queue_read_bind_stream(&stream, &reader) != 0) {
        return 1;
    }
    if (aeon_model_stream_seek(&stream, 8u) != 0) {
        return 1;
    }
    if (aeon_model_stream_read(&stream, aeon_nvme_queue_read_model_read,
                               &reader, out, sizeof(out)) != 0) {
        return 1;
    }
    if (memcmp(out, fake.disk + 8u, sizeof(out)) != 0 ||
        fake.lba != 2u || fake.count != 2u || fake.cid != 7u) {
        return 1;
    }
    if (aeon_model_stream_seek(&stream, 6u) != 0) {
        return 1;
    }
    if (aeon_model_stream_read(&stream, aeon_nvme_queue_read_model_read,
                               &reader, out, 4u) == 0) {
        return 1;
    }
    printf("AEON Batch 39 NVMe queue-backed model read test: PASS\n");
    return 0;
}
