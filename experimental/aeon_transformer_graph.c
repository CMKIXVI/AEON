/*
 * AEON GGUF -> transformer execution graph boundary.
 *
 * The graph intentionally binds to AEON's model-index/tensor runtime rather
 * than duplicating GGUF storage. This file is experimental until the actual
 * kernel/backend execution path is implemented and tested.
 */
#include <stddef.h>
#include <stdint.h>

#include "aeon_model_index.h"

typedef struct {
    uint32_t d_model;
    uint32_t n_heads;
    uint32_t head_dim;
    const aeon_model_tensor_meta_t *w_q;
    const aeon_model_tensor_meta_t *w_k;
    const aeon_model_tensor_meta_t *w_v;
    const aeon_model_tensor_meta_t *w_o;
} aeon_attention_graph_t;

typedef struct {
    uint32_t d_model;
    uint32_t max_seq_len;
    uint32_t n_heads;
} aeon_rope_graph_t;

typedef struct {
    uint32_t max_batch;
    uint32_t max_seq_len;
    uint32_t head_dim;
    void *k_cache;
    void *v_cache;
} aeon_kv_cache_graph_t;

typedef struct {
    uint32_t n_layers;
    uint32_t d_model;
    uint32_t vocab_size;
    uint32_t max_len;
    aeon_attention_graph_t *attention;
    aeon_rope_graph_t rope;
    aeon_kv_cache_graph_t kv;
    const aeon_model_tensor_meta_t *embedding;
    const aeon_model_tensor_meta_t *lm_head;
} aeon_transformer_graph_t;

static const aeon_model_tensor_meta_t *find_tensor(
    const aeon_model_index_t *index, const char *name)
{
    return aeon_model_index_find(index, name);
}

int aeon_transformer_graph_bind(aeon_transformer_graph_t *graph,
                                const aeon_model_index_t *index)
{
    if (graph == NULL || index == NULL) return -1;
    graph->embedding = find_tensor(index, "embed_tokens.weight");
    graph->lm_head = find_tensor(index, "lm_head.weight");
    return (graph->embedding != NULL && graph->lm_head != NULL) ? 0 : -2;
}

/* Execution remains backend-owned: RMSNorm -> RoPE/QKV -> KV cache ->
   attention -> residual -> SwiGLU -> residual -> final norm -> LM head. */
