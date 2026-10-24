#include "aeon_runtime.h"
#include <math.h>
#include <string.h>

static int phase(aeon_runtime_t *r, aeon_runtime_phase_t p) {
    if (!r || p >= 7) return -1;
    r->phase = p;
    r->phase_count[p]++;
    return 0;
}

int aeon_runtime_init(aeon_runtime_t *r, const aeon_runtime_config_t *config) {
    if (!r || !config || !config->action_count || config->action_count > 1024u ||
        config->model.hidden == 0 || config->model.heads == 0 ||
        config->model.head_dim == 0 || !isfinite(config->model.epsilon) ||
        config->model.epsilon <= 0.0f || config->actuator.max_command < 0.0f) return -1;
    memset(r, 0, sizeof(*r));
    r->config = *config;
    r->actuator = config->actuator;
    r->phase = AEON_PHASE_IDLE;
    if (aeon_detect_cpu_caps(&r->cpu) != 0) return -1;
    return 0;
}

int aeon_runtime_register_tool(aeon_runtime_t *r, uint32_t id, uint32_t capability,
                               aeon_tool_handler_t handler, void *context) {
    if (!r) return -1;
    return aeon_tool_register(&r->tools, id, capability, handler, context);
}

int aeon_runtime_invoke_tool(aeon_runtime_t *r, uint32_t id, uint32_t capability,
                             const void *input, size_t input_size, void *output,
                             size_t output_capacity, size_t *output_size) {
    if (!r) return -1;
    phase(r, AEON_PHASE_TOOLS);
    int rc = aeon_tool_invoke(&r->tools, id, capability, input, input_size,
                              output, output_capacity, output_size);
    r->last_status = rc;
    return rc;
}

int aeon_runtime_request_action(aeon_runtime_t *r, float command) {
    if (!r || !isfinite(command)) return -1;
    phase(r, AEON_PHASE_ACTION);
    int rc = aeon_action_gate(&r->actuator, command, &r->action);
    r->last_status = rc;
    return rc;
}

int aeon_runtime_tick(aeon_runtime_t *r, uint64_t timestamp) {
    if (!r) return -1;

    phase(r, AEON_PHASE_SENSORS);
    if (r->sensors.count == 0) {
        if (aeon_sensor_push(&r->sensors, 0u, timestamp, 0.0f) != 0) return -1;
    }

    phase(r, AEON_PHASE_COGNITION);
    float observation = r->sensors.samples[r->sensors.count - 1u].value;
    float error = fabsf(observation);
    if (aeon_active_inference(error, 1.0f, r->config.action_count,
                              &r->selected_action, &r->inference) != 0) return -1;
    if (aeon_workspace_publish(&r->workspace, r->ticks + 1u,
                               r->inference.confidence) != 0) return -1;
    if (aeon_memory_record(&r->memory, r->ticks + 1u, observation, 1.0f) != 0) return -1;

    phase(r, AEON_PHASE_MODEL);
    float x[4] = { observation, 1.0f, 0.0f, -observation };
    float norm[4];
    size_t n = r->config.model.hidden < 4u ? r->config.model.hidden : 4u;
    if (n == 0) return -1;
    if (aeon_rmsnorm(x, norm, n, r->config.model.epsilon) != 0) return -1;
    for (size_t i = 0; i < 4u; ++i) r->model_output[i] = i < n ? norm[i] : 0.0f;

    phase(r, AEON_PHASE_ACTION);
    float requested = (float)r->selected_action;
    if (aeon_action_gate(&r->actuator, requested, &r->action) != 0) return -1;

    phase(r, AEON_PHASE_TELEMETRY);
    r->ticks++;
    r->last_status = 0;
    r->phase = AEON_PHASE_IDLE;
    return 0;
}

int aeon_runtime_shutdown(aeon_runtime_t *r) {
    if (!r) return -1;
    r->phase = AEON_PHASE_IDLE;
    r->actuator.emergency_stop = 1u;
    r->action.approved = 0u;
    r->last_status = 0;
    return 0;
}
