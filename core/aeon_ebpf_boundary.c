#include "aeon_ebpf_boundary.h"
int aeon_ebpf_classify(const aeon_ebpf_event_t *event,aeon_ebpf_decision_t *decision)
{
    if(!event||!decision||event->process_id==0)return -1; decision->allow=0; decision->reason=AEON_EBPF_REASON_POLICY;
    if(!event->semantic_candidate){decision->reason=AEON_EBPF_REASON_NOT_SEMANTIC;return 0;}
    if(event->io_class!=AEON_IO_IPC&&event->io_class!=AEON_IO_NETWORK_TX&&event->io_class!=AEON_IO_NETWORK_RX)return -2;
    decision->reason=AEON_EBPF_REASON_IDENTITY_GATE; return 0;
}
