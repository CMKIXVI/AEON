#ifndef AEON_NVME_QUEUE_READ_H
#define AEON_NVME_QUEUE_READ_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_stream.h"

#define AEON_NVME_QUEUE_READ_MAGIC 0x41455152u
#define AEON_NVME_QUEUE_READ_VERSION 2u

typedef enum {
    AEON_NVME_READ_IDLE = 0,
    AEON_NVME_READ_SUBMITTED = 1,
    AEON_NVME_READ_COMPLETE = 2,
    AEON_NVME_READ_FAILED = 3
} aeon_nvme_read_state_t;

typedef int (*aeon_nvme_queue_read_submit_fn)(void *context, uint64_t lba,
                                               uint32_t lba_count, void *buffer,
                                               size_t buffer_bytes, uint16_t *cid);
typedef int (*aeon_nvme_queue_read_poll_fn)(void *context, uint16_t cid,
                                             int *completed, int *success);

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t cid;
    uint64_t offset;
    size_t length;
    aeon_nvme_read_state_t state;
    void *buffer;
} aeon_nvme_queue_read_request_t;

typedef struct {
    uint64_t capacity_bytes;
    uint32_t lba_bytes;
    aeon_nvme_queue_read_submit_fn submit;
    aeon_nvme_queue_read_poll_fn poll;
    void *context;
} aeon_nvme_queue_read_t;

int aeon_nvme_queue_read_init(aeon_nvme_queue_read_t *reader,
                              uint64_t capacity_bytes, uint32_t lba_bytes,
                              aeon_nvme_queue_read_submit_fn submit,
                              aeon_nvme_queue_read_poll_fn poll, void *context);
int aeon_nvme_queue_read_start(aeon_nvme_queue_read_t *reader,
                               aeon_nvme_queue_read_request_t *request,
                               size_t offset, void *buffer, size_t length);
int aeon_nvme_queue_read_complete(aeon_nvme_queue_read_t *reader,
                                  aeon_nvme_queue_read_request_t *request);
int aeon_nvme_queue_read_request(aeon_nvme_queue_read_t *reader,
                                 aeon_nvme_queue_read_request_t *request,
                                 size_t offset, void *buffer, size_t length);
int aeon_nvme_queue_read_model_read(void *context, size_t offset,
                                    void *buffer, size_t length);
int aeon_nvme_queue_read_bind_stream(aeon_model_stream_t *stream,
                                     aeon_nvme_queue_read_t *reader);

#endif
