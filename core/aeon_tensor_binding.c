#include "aeon_tensor_binding.h"

#include <stdint.h>
#include <limits.h>

int aeon_tensor_binding_validate(const aeon_tensor_binding_t *binding)
{
    if (binding == NULL || binding->tensor == NULL || binding->stream == NULL ||
        binding->bytes == 0u || binding->position > binding->bytes) {
        return -1;
    }
    if (aeon_model_stream_validate(binding->stream) != 0 ||
        binding->offset > binding->stream->total_bytes ||
        binding->bytes > binding->stream->total_bytes - binding->offset) {
        return -1;
    }
    if (binding->tensor->offset != (uint64_t)binding->offset ||
        binding->tensor->bytes != (uint64_t)binding->bytes) {
        return -1;
    }
    return 0;
}

int aeon_tensor_binding_init(aeon_tensor_binding_t *binding,
                              aeon_model_stream_t *stream,
                              const aeon_model_index_t *index,
                              const char *name)
{
    const aeon_model_tensor_meta_t *tensor;

    if (binding == NULL || stream == NULL || index == NULL || name == NULL ||
        aeon_model_stream_validate(stream) != 0 ||
        aeon_model_index_validate(index, (uint64_t)stream->total_bytes) != 0) {
        return -1;
    }
    tensor = aeon_model_index_find(index, name);
    if (tensor == NULL || aeon_tensor_layout_validate(tensor) != 0 ||
        tensor->offset > (uint64_t)SIZE_MAX || tensor->bytes > (uint64_t)SIZE_MAX) {
        return -1;
    }
    binding->tensor = tensor;
    binding->stream = stream;
    binding->offset = (size_t)tensor->offset;
    binding->bytes = (size_t)tensor->bytes;
    binding->position = 0u;
    return aeon_tensor_binding_validate(binding);
}

int aeon_tensor_binding_seek(aeon_tensor_binding_t *binding, size_t offset)
{
    if (aeon_tensor_binding_validate(binding) != 0 || offset > binding->bytes) {
        return -1;
    }
    binding->position = offset;
    return 0;
}

int aeon_tensor_binding_read(aeon_tensor_binding_t *binding, void *buffer, size_t length)
{
    size_t stream_offset;

    if (aeon_tensor_binding_validate(binding) != 0 || buffer == NULL || length == 0u ||
        length > binding->bytes - binding->position) {
        return -1;
    }
    if (binding->offset > SIZE_MAX - binding->position) {
        return -1;
    }
    stream_offset = binding->offset + binding->position;
    if (aeon_model_stream_seek(binding->stream, stream_offset) != 0 ||
        aeon_model_stream_read_bound(binding->stream, buffer, length) != 0) {
        return -1;
    }
    binding->position += length;
    return 0;
}

size_t aeon_tensor_binding_remaining(const aeon_tensor_binding_t *binding)
{
    if (aeon_tensor_binding_validate(binding) != 0) {
        return 0u;
    }
    return binding->bytes - binding->position;
}
