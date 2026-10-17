#include "aeon_kernel_adapter.h"
#include <string.h>
int aeon_kernel_hooks_attach(aeon_kernel_hook_stats_t *s){if(!s)return -1;memset(s,0,sizeof(*s));s->attached=1;return 0;}
int aeon_kernel_hooks_detach(aeon_kernel_hook_stats_t *s){if(!s||!s->attached)return -1;s->attached=0;return 0;}
int aeon_kernel_hook_event(aeon_kernel_hook_stats_t *s,const aeon_net_event_t *e,aeon_net_hook_t hook,const aeon_did_registry_t *registry,aeon_net_action_t *action)
{int rc;if(!s||!e||!action||!s->attached)return -1;rc=aeon_net_policy_decide(e,hook,registry,action);if(rc!=0)return rc;if(hook==AEON_NET_HOOK_PRE_TX)s->tx_events++;else s->rx_events++;if(*action==AEON_NET_DEFER_IDENTITY)s->deferred_identity++;if(*action==AEON_NET_DROP)s->dropped++;return 0;}
