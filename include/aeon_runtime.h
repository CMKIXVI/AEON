#ifndef AEON_RUNTIME_H
#define AEON_RUNTIME_H

#include <stddef.h>
#include <stdint.h>
#include "aeon_cognitive_engine.h"
#include "aeon_system_embodiment.h"
#include "aeon_transformer_compute.h"

typedef enum {
    AEON_PHASE_IDLE = 0,
    AEON_PHASE_SENSORS,
    AEON_PHASE_COGNITION,
    AEON_PHASE_MODEL,
    AEON_PHASE_TOOLS,
    AEON_PHASE_ACTION,
    AEON_PHASE_TELEMETRY
} aeon_runtime_phase_t;

typedef struct {
    aeon_transformer_config_t model;
    aeon_actuator_policy_t actuator;
    uint32_t tool_capability;
    size_t action_count;
} aeon_runtime_config_t;

typedef struct {
    aeon_runtime_config_t config;
    aeon_runtime_phase_t phase;
    uint64_t ticks;
    uint64_t phase_count[7];
    aeon_cpu_caps_t cpu;
    aeon_sensor_frame_t sensors;
    aeon_tool_bus_t tools;
    aeon_actuator_policy_t actuator;
    aeon_causal_graph_t causal;
    aeon_episodic_memory_t memory;
    aeon_global_workspace_t workspace;
    aeon_inference_state_t inference;
    float model_output[4];
    int32_t selected_action;
    aeon_action_command_t action;
    int last_status;
} aeon_runtime_t;

int aeon_runtime_init(aeon_runtime_t *runtime, const aeon_runtime_config_t *config);
int aeon_runtime_tick(aeon_runtime_t *runtime, uint64_t timestamp);
int aeon_runtime_shutdown(aeon_runtime_t *runtime);
int aeon_runtime_register_tool(aeon_runtime_t *runtime, uint32_t id,
                               uint32_t capability, aeon_tool_handler_t handler,
                               void *context);
int aeon_runtime_invoke_tool(aeon_runtime_t *runtime, uint32_t id,
                             uint32_t capability, const void *input,
                             size_t input_size, void *output,
                             size_t output_capacity, size_t *output_size);
int aeon_runtime_request_action(aeon_runtime_t *runtime, float command);

#endif
