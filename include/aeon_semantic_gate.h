#ifndef AEON_SEMANTIC_GATE_H
#define AEON_SEMANTIC_GATE_H
#include "aeon_semantic_frame.h"
typedef enum { AEON_SEMANTIC_ALLOW=0, AEON_SEMANTIC_DENY_MALFORMED=1, AEON_SEMANTIC_DENY_DIGEST=2, AEON_SEMANTIC_DENY_IDENTITY=3 } aeon_semantic_gate_reason_t;
int aeon_semantic_gate_check(const aeon_semantic_frame_t *, const aeon_did_registry_t *, aeon_semantic_gate_reason_t *);
#endif
