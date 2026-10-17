#include "aeon_process_gate.h"
#include <string.h>
static aeon_process_identity_state_t *find_state(aeon_process_gate_t *g,uint32_t pid)
{
    for (size_t i=0;i<AEON_MAX_PROCESS_STATES;++i) if(g->states[i].process_id==pid) return &g->states[i];
    for (size_t i=0;i<AEON_MAX_PROCESS_STATES;++i) if(g->states[i].process_id==0){g->states[i].process_id=pid;return &g->states[i];}
    return NULL;
}
int aeon_process_gate_init(aeon_process_gate_t *g){if(!g)return -1;memset(g,0,sizeof(*g));g->policy_generation=1;return 0;}
int aeon_process_gate_submit(aeon_process_gate_t *g,const aeon_did_registry_t *registry,const aeon_ioctl_submit_t *req,aeon_io_decision_t *decision)
{
    aeon_io_gate_request_t gate; aeon_submit_request_t submit; aeon_process_identity_state_t *state; int rc;
    if(!g||!registry||!req||!decision||req->abi_version!=AEON_IDENTITY_ABI_VERSION||req->process_id==0)return -1;
    state=find_state(g,req->process_id); if(!state)return -2;
    memset(&submit,0,sizeof(submit)); submit.abi_version=req->abi_version; submit.flags=req->flags; submit.transmission=req->transmission;
    gate.io_class=(aeon_io_class_t)req->io_class; gate.process_id=req->process_id; gate.flags=req->flags; gate.request=&submit;
    rc=aeon_io_gate_check(registry,&gate,decision);
    if(rc==0&&*decision==AEON_IO_ALLOW){state->authenticated=1;state->accepted++;return 0;}
    state->denied++;state->authenticated=0;*decision=AEON_IO_DENY;return rc==0?-3:rc;
}
int aeon_process_gate_query(const aeon_process_gate_t *g,uint32_t pid,aeon_ioctl_query_t *out)
{
    if(!g||!out||pid==0)return -1; memset(out,0,sizeof(*out)); out->abi_version=AEON_IDENTITY_ABI_VERSION; out->process_id=pid; out->policy_generation=g->policy_generation;
    for(size_t i=0;i<AEON_MAX_PROCESS_STATES;++i) if(g->states[i].process_id==pid){out->authenticated=g->states[i].authenticated;return 0;} return -2;
}
