#ifndef AEON_COGNITIVE_ENGINE_H
#define AEON_COGNITIVE_ENGINE_H

#include <stddef.h>
#include <stdint.h>

#define AEON_COGNITIVE_MAX_FACTS 64
#define AEON_COGNITIVE_MAX_MEMORY 32
#define AEON_COGNITIVE_MAX_WORKSPACE 16

typedef struct {
    uint64_t id;
    int32_t cause;
    int32_t effect;
    float strength;
    uint8_t active;
} aeon_causal_edge_t;

typedef struct {
    aeon_causal_edge_t edges[AEON_COGNITIVE_MAX_FACTS];
    size_t count;
} aeon_causal_graph_t;

typedef struct {
    uint64_t id;
    float value;
    float fisher;
    uint8_t valid;
} aeon_memory_item_t;

typedef struct {
    aeon_memory_item_t items[AEON_COGNITIVE_MAX_MEMORY];
    size_t count;
    size_t cursor;
} aeon_episodic_memory_t;

typedef struct {
    uint64_t ids[AEON_COGNITIVE_MAX_WORKSPACE];
    float salience[AEON_COGNITIVE_MAX_WORKSPACE];
    size_t count;
} aeon_global_workspace_t;

typedef struct {
    float prediction_error;
    float expected_free_energy;
    float confidence;
    int32_t selected_action;
} aeon_inference_state_t;

int aeon_vsa_cleanup(const int8_t *query, const int8_t *const *dictionary,
                     size_t dictionary_count, size_t dimensions,
                     size_t *best_index, float *best_score);
int aeon_causal_add(aeon_causal_graph_t *graph, uint64_t id,
                    int32_t cause, int32_t effect, float strength);
int aeon_causal_do(const aeon_causal_graph_t *graph, int32_t intervention,
                   int32_t target, float *effect_probability);
int aeon_active_inference(float prediction_error, float uncertainty,
                          size_t action_count, int32_t *selected_action,
                          aeon_inference_state_t *state);
int aeon_memory_record(aeon_episodic_memory_t *memory, uint64_t id,
                       float value, float fisher);
int aeon_memory_consolidate(aeon_episodic_memory_t *memory, float learning_rate,
                            float ewc_lambda);
int aeon_workspace_publish(aeon_global_workspace_t *workspace, uint64_t id,
                           float salience);
int aeon_workspace_select(const aeon_global_workspace_t *workspace,
                         uint64_t *id, float *salience);

#endif
