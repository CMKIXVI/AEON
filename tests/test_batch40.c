#include <stdio.h>
#include <string.h>
#include "aeon_nvme_queue_read.h"

typedef struct {
    unsigned char disk[32];
    uint64_t lba;
    uint32_t count;
    uint16_t cid;
    void *buffer;
    size_t bytes;
    int completed;
    int success;
} fake_t;

static int submit(void *context, uint64_t lba, uint32_t count,
                  void *buffer, size_t bytes, uint16_t *cid)
{
    fake_t *d = (fake_t *)context;
    if ((size_t)lba * 4u + bytes > sizeof(d->disk)) {
        return -1;
    }
    d->lba = lba;
    d->count = count;
    d->cid = 9u;
    d->buffer = buffer;
    d->bytes = bytes;
    d->completed = 0;
    d->success = 1;
    *cid = d->cid;
    return 0;
}

static int poll(void *context, uint16_t cid, int *completed, int *success)
{
    fake_t *d = (fake_t *)context;
    if (cid != d->cid) {
        return -1;
    }
    if (!d->completed) {
        memcpy(d->buffer, d->disk + (size_t)d->lba * 4u, d->bytes);
        d->completed = 1;
    }
    *completed = d->completed;
    *success = d->success;
    return 0;
}

int main(void)
{
    fake_t d;
    aeon_nvme_queue_read_t reader;
    aeon_nvme_queue_read_request_t request;
    unsigned char out[8];
    size_t i;

    for (i = 0u; i < sizeof(d.disk); ++i) {
        d.disk[i] = (unsigned char)i;
    }

    if (aeon_nvme_queue_read_init(&reader, sizeof(d.disk), 4u,
                                  submit, poll, &d) != 0) {
        return 1;
    }
    if (aeon_nvme_queue_read_start(&reader, &request, 8u,
                                   out, sizeof(out)) != 0) {
        return 1;
    }
    if (request.state != AEON_NVME_READ_SUBMITTED) {
        return 1;
    }
    if (aeon_nvme_queue_read_complete(&reader, &request) != 0) {
        return 1;
    }
    if (request.state != AEON_NVME_READ_COMPLETE ||
        memcmp(out, d.disk + 8u, sizeof(out)) != 0) {
        return 1;
    }
    if (aeon_nvme_queue_read_start(&reader, &request, 6u, out, 4u) == 0) {
        return 1;
    }

    printf("AEON Batch 40 asynchronous NVMe read lifecycle test: PASS\n");
    return 0;
}
