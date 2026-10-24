#include "aeon_cognitive_engine.h"
#include <math.h>

static float dot(const int8_t *a, const int8_t *b, size_t n) {
    int64_t s = 0;
    for (size_t i = 0; i < n; ++i) s += (int)a[i] * (int)b[i];
    return (float)s;
}

int aeon_vsa_cleanup(const int8_t *query, const int8_t *const *dictionary,
                     size_t dictionary_count, size_t dimensions,
                     size_t *best_index, float *best_score) {
    if (!query || !dictionary || !dictionary_count || !dimensions || !best_index || !best_score) return -1;
    float best = -INFINITY; size_t bi = 0;
    for (size_t i = 0; i < dictionary_count; ++i) {
        if (!dictionary[i]) return -1;
        float score = dot(query, dictionary[i], dimensions) / (float)dimensions;
        if (score > best) { best = score; bi = i; }
    }
    *best_index = bi; *best_score = best; return 0;
}

int aeon_causal_add(aeon_causal_graph_t *graph, uint64_t id,
                    int32_t cause, int32_t effect, float strength) {
    if (!graph || graph->count >= AEON_COGNITIVE_MAX_FACTS || !isfinite(strength)) return -1;
    aeon_causal_edge_t *e = &graph->edges[graph->count++];
    e->id = id; e->cause = cause; e->effect = effect; e->strength = strength; e->active = 1;
    return 0;
}

int aeon_causal_do(const aeon_causal_graph_t *graph, int32_t intervention,
                   int32_t target, float *effect_probability) {
    if (!graph || !effect_probability) return -1;
    float total = 0.0f, hit = 0.0f;
    for (size_t i = 0; i < graph->count; ++i) {
        const aeon_causal_edge_t *e = &graph->edges[i];
        if (!e->active || e->cause != intervention) continue;
        float w = fabsf(e->strength);
        total += w;
        if (e->effect == target) hit += w;
    }
    *effect_probability = total > 0.0f ? hit / total : 0.0f;
    return 0;
}

int aeon_active_inference(float prediction_error, float uncertainty,
                          size_t action_count, int32_t *selected_action,
                          aeon_inference_state_t *state) {
    if (!selected_action || !state || !action_count || prediction_error < 0.0f || uncertainty < 0.0f) return -1;
    float best = INFINITY; int32_t action = 0;
    for (size_t a = 0; a < action_count; ++a) {
        float risk = prediction_error * (1.0f + 0.1f * (float)a);
        float epistemic = uncertainty / (float)(a + 1u);
        float efe = risk + epistemic;
        if (efe < best) { best = efe; action = (int32_t)a; }
    }
    state->prediction_error = prediction_error;
    state->expected_free_energy = best;
    state->confidence = 1.0f / (1.0f + prediction_error + uncertainty);
    state->selected_action = action;
    *selected_action = action;
    return 0;
}

int aeon_memory_record(aeon_episodic_memory_t *memory, uint64_t id,
                       float value, float fisher) {
    if (!memory || !isfinite(value) || fisher < 0.0f) return -1;
    size_t slot = memory->count < AEON_COGNITIVE_MAX_MEMORY ? memory->count++ : memory->cursor;
    memory->items[slot].id = id; memory->items[slot].value = value;
    memory->items[slot].fisher = fisher; memory->items[slot].valid = 1;
    memory->cursor = (slot + 1u) % AEON_COGNITIVE_MAX_MEMORY;
    return 0;
}

int aeon_memory_consolidate(aeon_episodic_memory_t *memory, float learning_rate,
                            float ewc_lambda) {
    if (!memory || learning_rate < 0.0f || ewc_lambda < 0.0f) return -1;
    for (size_t i = 0; i < memory->count; ++i) {
        aeon_memory_item_t *m = &memory->items[i];
        if (!m->valid) continue;
        float penalty = ewc_lambda * m->fisher * m->value;
        m->value += learning_rate * (m->value - penalty);
    }
    return 0;
}

int aeon_workspace_publish(aeon_global_workspace_t *workspace, uint64_t id,
                           float salience) {
    if (!workspace || !isfinite(salience) || salience < 0.0f) return -1;
    size_t slot = workspace->count < AEON_COGNITIVE_MAX_WORKSPACE ? workspace->count++ : 0;
    if (workspace->count == AEON_COGNITIVE_MAX_WORKSPACE && slot == 0) {
        size_t lowest = 0;
        for (size_t i = 1; i < workspace->count; ++i) if (workspace->salience[i] < workspace->salience[lowest]) lowest = i;
        slot = lowest;
    }
    workspace->ids[slot] = id; workspace->salience[slot] = salience;
    return 0;
}

int aeon_workspace_select(const aeon_global_workspace_t *workspace,
                          uint64_t *id, float *salience) {
    if (!workspace || !id || !salience || !workspace->count) return -1;
    size_t best = 0;
    for (size_t i = 1; i < workspace->count; ++i) if (workspace->salience[i] > workspace->salience[best]) best = i;
    *id = workspace->ids[best]; *salience = workspace->salience[best];
    return 0;
}
