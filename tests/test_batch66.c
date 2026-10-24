#include "aeon_system_embodiment.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int echo_tool(const void *in, size_t n, void *out, size_t cap, size_t *written, void *ctx) {
    (void)ctx;
    if (!in || !out || !written || n > cap) return -1;
    memcpy(out, in, n); *written = n; return 0;
}

int main(void) {
    aeon_cpu_caps_t caps;
    assert(aeon_detect_cpu_caps(&caps) == 0);
    assert(caps.arch != AEON_ARCH_UNKNOWN);

    aeon_memory_region_t region = {4096, 4096, 1};
    assert(aeon_validate_memory_region(&region) == 0);

    aeon_sensor_frame_t frame = {0};
    assert(aeon_sensor_push(&frame, 1, 10, 0.25f) == 0);
    assert(frame.count == 1 && frame.samples[0].valid);

    aeon_tool_bus_t bus = {0};
    assert(aeon_tool_register(&bus, 7, 0x4u, echo_tool, NULL) == 0);
    char in[] = "aeon"; char out[8] = {0}; size_t written = 0;
    assert(aeon_tool_invoke(&bus, 7, 0x4u, in, 5, out, sizeof(out), &written) == 0);
    assert(written == 5 && memcmp(in, out, 5) == 0);
    assert(aeon_tool_invoke(&bus, 7, 0x2u, in, 5, out, sizeof(out), &written) == -2);

    aeon_actuator_policy_t policy = {1, 0, 1.0f};
    aeon_action_command_t command;
    assert(aeon_action_gate(&policy, 0.5f, &command) == 0 && command.approved);
    assert(aeon_action_gate(&policy, 2.0f, &command) == 0 && !command.approved);
    policy.emergency_stop = 1;
    assert(aeon_action_gate(&policy, 0.5f, &command) == 0 && !command.approved);

    puts("AEON Batch 66 system and embodiment: PASS");
    return 0;
}
