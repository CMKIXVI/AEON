#ifndef AEON_TENSOR_BINDING_H
#define AEON_TENSOR_BINDING_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_model_index.h"
#include "aeon_model_stream.h"

typedef struct {
    const aeon_model_tensor_meta_t *tensor;
    aeon_model_stream_t *stream;
    size_t offset;
    size_t bytes;
    size_t position;
} aeon_tensor_binding_t;

int aeon_tensor_binding_init(aeon_tensor_binding_t *binding,
                              aeon_model_stream_t *stream,
                              const aeon_model_index_t *index,
                              const char *name);
int aeon_tensor_binding_validate(const aeon_tensor_binding_t *binding);
int aeon_tensor_binding_seek(aeon_tensor_binding_t *binding, size_t offset);
int aeon_tensor_binding_read(aeon_tensor_binding_t *binding, void *buffer, size_t length);
size_t aeon_tensor_binding_remaining(const aeon_tensor_binding_t *binding);

#endif
