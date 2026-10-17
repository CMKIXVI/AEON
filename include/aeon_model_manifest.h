#ifndef AEON_MODEL_MANIFEST_H
#define AEON_MODEL_MANIFEST_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_index.h"
#include "aeon_model_stream.h"

#define AEON_MODEL_MANIFEST_MAGIC 0x41454D46u
#define AEON_MODEL_MANIFEST_VERSION 1u

int aeon_model_manifest_parse(aeon_model_index_t *index, const char *text,
                              size_t length, uint64_t model_bytes);
int aeon_model_manifest_load_tensor(aeon_model_stream_t *stream,
                                    const aeon_model_index_t *index,
                                    const char *name, void *buffer,
                                    size_t buffer_bytes);

#endif
