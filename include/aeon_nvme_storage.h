#ifndef AEON_NVME_STORAGE_H
#define AEON_NVME_STORAGE_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_stream.h"

typedef int (*aeon_nvme_read_lba_fn)(void *context, uint64_t lba,
                                     size_t lba_count, void *buffer);

typedef struct {
    uint64_t capacity_bytes;
    uint32_t lba_bytes;
    aeon_nvme_read_lba_fn read_lba;
    void *context;
} aeon_nvme_storage_t;

int aeon_nvme_storage_init(aeon_nvme_storage_t *storage,
                           uint64_t capacity_bytes, uint32_t lba_bytes,
                           aeon_nvme_read_lba_fn read_lba, void *context);
int aeon_nvme_storage_read(void *context, size_t offset, void *buffer, size_t length);
int aeon_nvme_storage_bind_stream(aeon_model_stream_t *stream,
                                  aeon_nvme_storage_t *storage);

#endif
