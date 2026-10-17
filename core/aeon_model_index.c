#include "aeon_model_index.h"

#include <string.h>

static int aeon_model_index_valid_dtype(aeon_model_dtype_t dtype)
{
    return dtype >= AEON_MODEL_DTYPE_F32 && dtype <= AEON_MODEL_DTYPE_I8;
}

static int aeon_model_index_range_ok(uint64_t offset, uint64_t bytes, uint64_t total)
{
    return offset <= total && bytes <= total - offset;
}

int aeon_model_index_init(aeon_model_index_t *index)
{
    if (index == NULL) {
        return -1;
    }
    memset(index, 0, sizeof(*index));
    index->magic = AEON_MODEL_INDEX_MAGIC;
    index->version = AEON_MODEL_INDEX_VERSION;
    return 0;
}

int aeon_model_index_add(aeon_model_index_t *index, const char *name,
                         uint64_t offset, uint64_t bytes,
                         aeon_model_dtype_t dtype, uint8_t rank,
                         const uint64_t *shape)
{
    size_t i;
    size_t name_length;

    if (index == NULL || name == NULL || shape == NULL ||
        index->magic != AEON_MODEL_INDEX_MAGIC ||
        index->version != AEON_MODEL_INDEX_VERSION ||
        index->count >= AEON_MODEL_INDEX_MAX_TENSORS ||
        !aeon_model_index_valid_dtype(dtype) || rank == 0u ||
        rank > AEON_MODEL_INDEX_MAX_DIMS || bytes == 0u) {
        return -1;
    }

    name_length = strlen(name);
    if (name_length == 0u || name_length >= AEON_MODEL_INDEX_NAME_MAX) {
        return -1;
    }

    for (i = 0u; i < index->count; ++i) {
        if (strcmp(index->tensors[i].name, name) == 0) {
            return -2;
        }
    }

    for (i = 0u; i < rank; ++i) {
        if (shape[i] == 0u) {
            return -3;
        }
    }

    memset(&index->tensors[index->count], 0, sizeof(index->tensors[index->count]));
    memcpy(index->tensors[index->count].name, name, name_length + 1u);
    index->tensors[index->count].offset = offset;
    index->tensors[index->count].bytes = bytes;
    index->tensors[index->count].dtype = dtype;
    index->tensors[index->count].rank = rank;
    for (i = 0u; i < rank; ++i) {
        index->tensors[index->count].shape[i] = shape[i];
    }
    index->count += 1u;
    return 0;
}

int aeon_model_index_validate(const aeon_model_index_t *index, uint64_t model_bytes)
{
    size_t i;
    size_t j;

    if (index == NULL || index->magic != AEON_MODEL_INDEX_MAGIC ||
        index->version != AEON_MODEL_INDEX_VERSION ||
        index->count > AEON_MODEL_INDEX_MAX_TENSORS || model_bytes == 0u) {
        return -1;
    }

    for (i = 0u; i < index->count; ++i) {
        const aeon_model_tensor_meta_t *tensor = &index->tensors[i];

        if (tensor->name[0] == '\0' ||
            !aeon_model_index_valid_dtype(tensor->dtype) ||
            tensor->rank == 0u || tensor->rank > AEON_MODEL_INDEX_MAX_DIMS ||
            !aeon_model_index_range_ok(tensor->offset, tensor->bytes, model_bytes)) {
            return -2;
        }
        for (j = 0u; j < tensor->rank; ++j) {
            if (tensor->shape[j] == 0u) {
                return -3;
            }
        }

        for (j = i + 1u; j < index->count; ++j) {
            const aeon_model_tensor_meta_t *other = &index->tensors[j];
            if (strcmp(tensor->name, other->name) == 0) {
                return -4;
            }
            if (tensor->offset < other->offset) {
                if (tensor->bytes > other->offset - tensor->offset) {
                    return -5;
                }
            } else if (other->offset < tensor->offset) {
                if (other->bytes > tensor->offset - other->offset) {
                    return -5;
                }
            } else {
                return -5;
            }
        }
    }
    return 0;
}

const aeon_model_tensor_meta_t *aeon_model_index_find(const aeon_model_index_t *index,
                                                       const char *name)
{
    size_t i;

    if (index == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0u; i < index->count; ++i) {
        if (strcmp(index->tensors[i].name, name) == 0) {
            return &index->tensors[i];
        }
    }
    return NULL;
}
