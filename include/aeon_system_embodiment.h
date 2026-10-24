#ifndef AEON_SYSTEM_EMBODIMENT_H
#define AEON_SYSTEM_EMBODIMENT_H

#include <stddef.h>
#include <stdint.h>
#define AEON_SENSOR_MAX 16u
#define AEON_TOOL_MAX 16u
typedef enum { AEON_ARCH_UNKNOWN = 0, AEON_ARCH_X86_64, AEON_ARCH_ARM64 } aeon_arch_t;
typedef struct { aeon_arch_t arch; uint32_t simd_bits; uint8_t avx2, avx512f, vnni, neon; } aeon_cpu_caps_t;
typedef struct { uint64_t bytes; uint32_t alignment; uint8_t writable; } aeon_memory_region_t;
typedef struct { uint64_t timestamp; float value; uint32_t kind; uint8_t valid; } aeon_sensor_sample_t;
typedef struct { aeon_sensor_sample_t samples[AEON_SENSOR_MAX]; size_t count; } aeon_sensor_frame_t;
typedef int (*aeon_tool_handler_t)(const void *, size_t, void *, size_t, size_t *, void *);
typedef struct { uint32_t id, required_capability; aeon_tool_handler_t handler; void *context; } aeon_tool_entry_t;
typedef struct { aeon_tool_entry_t entries[AEON_TOOL_MAX]; size_t count; } aeon_tool_bus_t;
typedef struct { uint8_t motors_enabled, emergency_stop; float max_command; } aeon_actuator_policy_t;
typedef struct { float command; uint8_t approved; } aeon_action_command_t;
int aeon_detect_cpu_caps(aeon_cpu_caps_t *caps);
int aeon_validate_memory_region(const aeon_memory_region_t *region);
int aeon_sensor_push(aeon_sensor_frame_t *frame, uint32_t kind, uint64_t timestamp, float value);
int aeon_tool_register(aeon_tool_bus_t *bus, uint32_t id, uint32_t capability, aeon_tool_handler_t handler, void *context);
int aeon_tool_invoke(const aeon_tool_bus_t *bus, uint32_t id, uint32_t capability, const void *input, size_t input_size, void *output, size_t output_capacity, size_t *output_size);
int aeon_action_gate(const aeon_actuator_policy_t *policy, float requested, aeon_action_command_t *command);
#endif
