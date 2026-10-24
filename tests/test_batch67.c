#include "aeon_runtime.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int echo_tool(const void *in, size_t in_size, void *out, size_t out_cap,
                     size_t *out_size, void *ctx) {
    (void)ctx;
    if (!in || !out || !out_size || in_size > out_cap) return -1;
    memcpy(out, in, in_size);
    *out_size = in_size;
    return 0;
}

int main(void) {
    aeon_runtime_config_t cfg = {
        .model = {4u, 1u, 4u, 1e-5f},
        .actuator = {1u, 0u, 3.0f},
        .tool_capability = 0x1u,
        .action_count = 4u
    };
    aeon_runtime_t r;
    assert(aeon_runtime_init(&r, &cfg) == 0);
    assert(aeon_validate_memory_region(&(aeon_memory_region_t){4096u, 64u, 1u}) == 0);
    assert(aeon_runtime_register_tool(&r, 7u, 0x1u, echo_tool, NULL) == 0);

    const char input[] = "aeon";
    char output[16] = {0};
    size_t output_size = 0;
    assert(aeon_runtime_invoke_tool(&r, 7u, 0x1u, input, sizeof(input),
                                    output, sizeof(output), &output_size) == 0);
    assert(output_size == sizeof(input));
    assert(memcmp(input, output, sizeof(input)) == 0);
    assert(aeon_runtime_invoke_tool(&r, 7u, 0x0u, input, sizeof(input),
                                    output, sizeof(output), &output_size) == -2);

    assert(aeon_sensor_push(&r.sensors, 1u, 100u, 0.25f) == 0);
    assert(aeon_runtime_tick(&r, 101u) == 0);
    assert(r.ticks == 1u);
    assert(r.phase_count[AEON_PHASE_SENSORS] == 1u);
    assert(r.phase_count[AEON_PHASE_COGNITION] == 1u);
    assert(r.phase_count[AEON_PHASE_MODEL] == 1u);
    assert(r.phase_count[AEON_PHASE_ACTION] == 1u);
    assert(r.phase_count[AEON_PHASE_TELEMETRY] == 1u);
    assert(r.action.approved == 1u);

    assert(aeon_runtime_request_action(&r, 4.0f) == 0);
    assert(r.action.approved == 0u);
    r.actuator.emergency_stop = 1u;
    assert(aeon_runtime_request_action(&r, 1.0f) == 0);
    assert(r.action.approved == 0u);
    assert(aeon_runtime_shutdown(&r) == 0);
    printf("AEON Batch 67 final integration: PASS\n");
    return 0;
}
