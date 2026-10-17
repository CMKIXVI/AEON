#include <stdio.h>
#include <string.h>
#include "aeon_model_stream.h"
#include "aeon_nvme_queue_read.h"

typedef struct { unsigned char disk[32]; uint64_t lba; uint32_t count; uint16_t cid; int completed; int success; } fake_nvme_t;

static int submit(void *context, uint64_t lba, uint32_t count, void *buffer, size_t bytes, uint16_t *cid)
{
    fake_nvme_t *d = (fake_nvme_t *)context;
    size_t offset = (size_t)lba * 4u;
    if (offset + bytes > sizeof(d->disk) || count == 0u || bytes != (size_t)count * 4u) return -1;
    memcpy(buffer, d->disk + offset, bytes);
    d->lba = lba; d->count = count; d->cid = 11u; *cid = d->cid;
    return 0;
}

static int poll(void *context, uint16_t cid, int *completed, int *success)
{
    fake_nvme_t *d = (fake_nvme_t *)context;
    if (cid != d->cid) return -1;
    *completed = d->completed; *success = d->success; return 0;
}

int main(void)
{
    fake_nvme_t fake = {0};
    aeon_nvme_queue_read_t reader;
    aeon_model_stream_t stream;
    unsigned char out[8];
    size_t i;
    for (i = 0u; i < sizeof(fake.disk); ++i) fake.disk[i] = (unsigned char)i;
    fake.completed = 1; fake.success = 1;
    if (aeon_nvme_queue_read_init(&reader, sizeof(fake.disk), 4u, submit, poll, &fake) != 0) return 1;
    if (aeon_nvme_queue_read_bind_stream(&stream, &reader) != 0) return 1;
    if (stream.reader == NULL || stream.context != &reader) return 1;
    if (aeon_model_stream_seek(&stream, 8u) != 0) return 1;
    if (aeon_model_stream_read_bound(&stream, out, sizeof(out)) != 0) return 1;
    if (memcmp(out, fake.disk + 8u, sizeof(out)) != 0 || fake.lba != 2u || fake.count != 2u || fake.cid != 11u) return 1;
    if (aeon_model_stream_seek(&stream, 6u) != 0) return 1;
    if (aeon_model_stream_read_bound(&stream, out, 4u) == 0) return 1;
    aeon_model_stream_t unbound;
    if (aeon_model_stream_init(&unbound, sizeof(fake.disk)) != 0) return 1;
    if (aeon_model_stream_read_bound(&unbound, out, 4u) == 0) return 1;
    fake.completed = 0;
    if (aeon_model_stream_seek(&stream, 8u) != 0) return 1;
    if (aeon_model_stream_read_bound(&stream, out, 4u) == 0) return 1;
    printf("AEON Batch 41 bound model stream test: PASS\n");
    return 0;
}
