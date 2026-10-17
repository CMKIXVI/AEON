#include "aeon_nvme_queue_read.h"

#include <limits.h>
#include <stdint.h>

static int add_overflow_size(size_t a, size_t b, size_t *out)
{
    if (b > SIZE_MAX - a) {
        return -1;
    }
    *out = a + b;
    return 0;
}

int aeon_nvme_queue_read_init(aeon_nvme_queue_read_t *reader,
                              uint64_t capacity_bytes, uint32_t lba_bytes,
                              aeon_nvme_queue_read_submit_fn submit,
                              aeon_nvme_queue_read_poll_fn poll, void *context)
{
    if (reader == NULL || capacity_bytes == 0u || lba_bytes == 0u ||
        (lba_bytes & (lba_bytes - 1u)) != 0u ||
        capacity_bytes % lba_bytes != 0u || submit == NULL || poll == NULL) {
        return -1;
    }

    reader->capacity_bytes = capacity_bytes;
    reader->lba_bytes = lba_bytes;
    reader->submit = submit;
    reader->poll = poll;
    reader->context = context;
    return 0;
}

int aeon_nvme_queue_read_start(aeon_nvme_queue_read_t *reader,
                               aeon_nvme_queue_read_request_t *request,
                               size_t offset, void *buffer, size_t length)
{
    size_t end;
    size_t lba_count_size;
    uint64_t lba;
    uint16_t cid = 0u;

    if (reader == NULL || request == NULL || buffer == NULL || length == 0u ||
        offset % reader->lba_bytes != 0u || length % reader->lba_bytes != 0u ||
        add_overflow_size(offset, length, &end) != 0 ||
        (uint64_t)end > reader->capacity_bytes) {
        return -1;
    }

    lba = (uint64_t)offset / reader->lba_bytes;
    lba_count_size = length / reader->lba_bytes;
    if (lba_count_size > UINT32_MAX) {
        return -1;
    }

    request->magic = AEON_NVME_QUEUE_READ_MAGIC;
    request->version = AEON_NVME_QUEUE_READ_VERSION;
    request->cid = 0u;
    request->offset = offset;
    request->length = length;
    request->state = AEON_NVME_READ_IDLE;
    request->buffer = buffer;

    if (reader->submit(reader->context, lba, (uint32_t)lba_count_size,
                       buffer, length, &cid) != 0) {
        request->state = AEON_NVME_READ_FAILED;
        return -1;
    }

    request->cid = cid;
    request->state = AEON_NVME_READ_SUBMITTED;
    return 0;
}

int aeon_nvme_queue_read_complete(aeon_nvme_queue_read_t *reader,
                                  aeon_nvme_queue_read_request_t *request)
{
    int completed = 0;
    int success = 0;

    if (reader == NULL || request == NULL ||
        request->magic != AEON_NVME_QUEUE_READ_MAGIC ||
        request->version != AEON_NVME_QUEUE_READ_VERSION ||
        request->state != AEON_NVME_READ_SUBMITTED) {
        return -1;
    }

    if (reader->poll(reader->context, request->cid,
                     &completed, &success) != 0) {
        return -1;
    }

    if (completed == 0) {
        return 1;
    }

    request->state = success != 0 ? AEON_NVME_READ_COMPLETE
                                  : AEON_NVME_READ_FAILED;
    return success != 0 ? 0 : -1;
}

int aeon_nvme_queue_read_request(aeon_nvme_queue_read_t *reader,
                                 aeon_nvme_queue_read_request_t *request,
                                 size_t offset, void *buffer, size_t length)
{
    int rc = aeon_nvme_queue_read_start(reader, request, offset, buffer, length);
    if (rc != 0) {
        return rc;
    }

    rc = aeon_nvme_queue_read_complete(reader, request);
    return rc == 1 ? -1 : rc;
}

int aeon_nvme_queue_read_model_read(void *context, size_t offset,
                                    void *buffer, size_t length)
{
    aeon_nvme_queue_read_t *reader = (aeon_nvme_queue_read_t *)context;
    aeon_nvme_queue_read_request_t request;
    return aeon_nvme_queue_read_request(reader, &request, offset, buffer, length);
}

int aeon_nvme_queue_read_bind_stream(aeon_model_stream_t *stream,
                                     aeon_nvme_queue_read_t *reader)
{
    if (stream == NULL || reader == NULL || reader->capacity_bytes > SIZE_MAX) {
        return -1;
    }
    return aeon_model_stream_init(stream, (size_t)reader->capacity_bytes);
}
