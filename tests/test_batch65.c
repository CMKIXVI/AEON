#include "aeon_cognitive_engine.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const int8_t a[] = {1,1,1,1};
    const int8_t b[] = {1,1,1,1};
    const int8_t c[] = {-1,-1,-1,-1};
    const int8_t *dict[] = {c,b};
    size_t best; float score;
    assert(aeon_vsa_cleanup(a, dict, 2, 4, &best, &score) == 0);
    assert(best == 1 && score > 0.99f);

    aeon_causal_graph_t graph = {0};
    assert(aeon_causal_add(&graph, 1, 10, 20, 0.8f) == 0);
    assert(aeon_causal_add(&graph, 2, 10, 30, 0.2f) == 0);
    float p = 0.0f;
    assert(aeon_causal_do(&graph, 10, 20, &p) == 0);
    assert(fabsf(p - 0.8f) < 0.001f);

    aeon_inference_state_t inf = {0}; int32_t action = -1;
    assert(aeon_active_inference(0.5f, 0.4f, 4, &action, &inf) == 0);
    assert(action == 2 && inf.selected_action == 2);
    assert(inf.confidence > 0.0f && inf.confidence <= 1.0f);

    aeon_episodic_memory_t memory = {0};
    assert(aeon_memory_record(&memory, 7, 2.0f, 0.1f) == 0);
    assert(aeon_memory_consolidate(&memory, 0.1f, 0.5f) == 0);
    assert(memory.items[0].valid && isfinite(memory.items[0].value));

    aeon_global_workspace_t ws = {0}; uint64_t id; float sal;
    assert(aeon_workspace_publish(&ws, 11, 0.2f) == 0);
    assert(aeon_workspace_publish(&ws, 22, 0.9f) == 0);
    assert(aeon_workspace_select(&ws, &id, &sal) == 0);
    assert(id == 22 && fabsf(sal - 0.9f) < 0.001f);

    puts("AEON Batch 65 cognitive engine: PASS");
    return 0;
}
