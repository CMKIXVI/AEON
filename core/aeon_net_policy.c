#include "aeon_net_policy.h"
int aeon_net_policy_decide(const aeon_net_event_t *e,aeon_net_hook_t hook,const aeon_did_registry_t *registry,aeon_net_action_t *action)
{
    aeon_semantic_gate_reason_t reason;
    if(!e||!action||!registry||e->process_id==0)return -1; *action=AEON_NET_DROP;
    if(hook!=AEON_NET_HOOK_PRE_TX&&hook!=AEON_NET_HOOK_PRE_RX)return -2;
    if(!e->semantic_candidate){*action=AEON_NET_ACCEPT;return 0;}
    if(!e->semantic_frame){*action=AEON_NET_DEFER_IDENTITY;return 0;}
    if(aeon_semantic_gate_check(e->semantic_frame,registry,&reason)!=0)return -3;
    if(reason!=AEON_SEMANTIC_ALLOW){*action=AEON_NET_DROP;return 0;} *action=AEON_NET_ACCEPT;return 0;
}
