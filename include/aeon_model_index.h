#ifndef AEON_MODEL_INDEX_H
#define AEON_MODEL_INDEX_H

#include <stddef.h>
#include <stdint.h>

#define AEON_MODEL_INDEX_MAGIC 0x41454D49u
#define AEON_MODEL_INDEX_VERSION 1u
#define AEON_MODEL_INDEX_MAX_TENSORS 4096u
#define AEON_MODEL_INDEX_NAME_MAX 96u
#define AEON_MODEL_INDEX_MAX_DIMS 8u

typedef enum {
    AEON_MODEL_DTYPE_F32 = 1,
    AEON_MODEL_DTYPE_BF16 = 2,
    AEON_MODEL_DTYPE_F16 = 3,
    AEON_MODEL_DTYPE_FP8_E4M3 = 4,
    AEON_MODEL_DTYPE_I8 = 5
} aeon_model_dtype_t;

typedef struct {
    char name[AEON_MODEL_INDEX_NAME_MAX];
    uint64_t offset;
    uint64_t bytes;
    aeon_model_dtype_t dtype;
    uint8_t rank;
    uint64_t shape[AEON_MODEL_INDEX_MAX_DIMS];
} aeon_model_tensor_meta_t;

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t reserved;
    size_t count;
    aeon_model_tensor_meta_t tensors[AEON_MODEL_INDEX_MAX_TENSORS];
} aeon_model_index_t;

int aeon_model_index_init(aeon_model_index_t *index);
int aeon_model_index_validate(const aeon_model_index_t *index, uint64_t model_bytes);
int aeon_model_index_add(aeon_model_index_t *index, const char *name,
                         uint64_t offset, uint64_t bytes,
                         aeon_model_dtype_t dtype, uint8_t rank,
                         const uint64_t *shape);
const aeon_model_tensor_meta_t *aeon_model_index_find(const aeon_model_index_t *index,
                                                      const char *name);

#endif
