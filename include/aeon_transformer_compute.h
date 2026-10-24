#ifndef AEON_TRANSFORMER_COMPUTE_H
#define AEON_TRANSFORMER_COMPUTE_H

#include <stddef.h>

typedef struct {
    size_t hidden;
    size_t heads;
    size_t head_dim;
    float epsilon;
} aeon_transformer_config_t;

int aeon_rmsnorm(const float *x, float *out, size_t n, float epsilon);
int aeon_rope(float *q, float *k, size_t heads, size_t head_dim, size_t position);
int aeon_attention(const float *q, const float *k, const float *v, size_t tokens, size_t dim, float scale, float *out);
int aeon_swiglu(const float *gate, const float *up, float *out, size_t n);
int aeon_softmax(float *x, size_t n);

#endif
