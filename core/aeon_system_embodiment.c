#include "aeon_system_embodiment.h"
#include <math.h>
#include <string.h>

int aeon_detect_cpu_caps(aeon_cpu_caps_t *caps) {
    if (!caps) return -1;
    memset(caps, 0, sizeof(*caps));
#if defined(__x86_64__) || defined(_M_X64)
    caps->arch = AEON_ARCH_X86_64;
    caps->simd_bits = 128;
#if defined(__AVX2__)
    caps->avx2 = 1; caps->simd_bits = 256;
#endif
#if defined(__AVX512F__)
    caps->avx512f = 1; caps->simd_bits = 512;
#endif
#if defined(__AVX512VNNI__)
    caps->vnni = 1;
#endif
#elif defined(__aarch64__) || defined(_M_ARM64)
    caps->arch = AEON_ARCH_ARM64;
    caps->simd_bits = 128;
    caps->neon = 1;
#endif
    return 0;
}

int aeon_validate_memory_region(const aeon_memory_region_t *region) {
    if (!region || !region->bytes || !region->alignment) return -1;
    if ((region->alignment & (region->alignment - 1u)) != 0u) return -1;
    return 0;
}

int aeon_sensor_push(aeon_sensor_frame_t *frame, uint32_t kind, uint64_t timestamp, float value) {
    if (!frame || !isfinite(value)) return -1;
    size_t slot = frame->count < AEON_SENSOR_MAX ? frame->count++ : 0u;
    frame->samples[slot].timestamp = timestamp;
    frame->samples[slot].kind = kind;
    frame->samples[slot].value = value;
    frame->samples[slot].valid = 1;
    return 0;
}

int aeon_tool_register(aeon_tool_bus_t *bus, uint32_t id, uint32_t capability,
                       aeon_tool_handler_t handler, void *context) {
    if (!bus || !handler || bus->count >= AEON_TOOL_MAX) return -1;
    for (size_t i = 0; i < bus->count; ++i) if (bus->entries[i].id == id) return -1;
    bus->entries[bus->count++] = (aeon_tool_entry_t){id, capability, handler, context};
    return 0;
}

int aeon_tool_invoke(const aeon_tool_bus_t *bus, uint32_t id, uint32_t capability,
                     const void *input, size_t input_size, void *output,
                     size_t output_capacity, size_t *output_size) {
    if (!bus || !output_size) return -1;
    for (size_t i = 0; i < bus->count; ++i) {
        const aeon_tool_entry_t *e = &bus->entries[i];
        if (e->id == id) {
            if ((e->required_capability & capability) != e->required_capability) return -2;
            return e->handler(input, input_size, output, output_capacity, output_size, e->context);
        }
    }
    return -3;
}

int aeon_action_gate(const aeon_actuator_policy_t *policy, float requested,
                     aeon_action_command_t *command) {
    if (!policy || !command || !isfinite(requested)) return -1;
    command->command = 0.0f; command->approved = 0;
    if (policy->emergency_stop || !policy->motors_enabled) return 0;
    if (!isfinite(policy->max_command) || policy->max_command < 0.0f) return -1;
    if (fabsf(requested) > policy->max_command) return 0;
    command->command = requested;
    command->approved = 1;
    return 0;
}
