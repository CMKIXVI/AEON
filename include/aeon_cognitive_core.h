#ifndef AEON_COGNITIVE_CORE_H
#define AEON_COGNITIVE_CORE_H

#include <stddef.h>
#include <stdint.h>

#define AEON_VSA_BITS 2048u
#define AEON_VSA_WORDS 32u
#define AEON_SNN_NODES 1024u
#define AEON_SNN_SYNAPSES 32u

typedef struct { uint64_t w[AEON_VSA_WORDS]; } aeon_hv_t;

typedef struct { float v, u, a, b, c, d; uint32_t spikes; } aeon_snn_node_t;
typedef struct { aeon_snn_node_t node[AEON_SNN_NODES]; float branching_ratio; float dopamine; float norepinephrine; } aeon_snn_t;

typedef struct { float prediction_error; float free_energy; float expected_free_energy; float confidence; } aeon_inference_state_t;

void aeon_vsa_zero(aeon_hv_t *v);
void aeon_vsa_bind(aeon_hv_t *out, const aeon_hv_t *a, const aeon_hv_t *b);
void aeon_vsa_bundle(aeon_hv_t *out, const aeon_hv_t *const *items, size_t count);
void aeon_vsa_permute(aeon_hv_t *out, const aeon_hv_t *in, unsigned shift);
unsigned aeon_vsa_hamming(const aeon_hv_t *a, const aeon_hv_t *b);

int aeon_snn_init(aeon_snn_t *snn);
int aeon_snn_step(aeon_snn_t *snn, float input, float reward);

void aeon_inference_update(aeon_inference_state_t *s, float prediction, float observation, float uncertainty, float goal_error);
int aeon_q4_0_unpack(const uint8_t *packed, size_t count, int8_t *out);
int aeon_q4_0_gemv(const uint8_t *weights, const float *x, size_t rows, size_t cols, float scale, float *out);

#endif
