#include "aeon_io_gate.h"
int aeon_io_gate_check(const aeon_did_registry_t *registry,const aeon_io_gate_request_t *req,aeon_io_decision_t *decision)
{
    int rc;
    if (!registry || !req || !decision || !req->request || req->process_id==0) return -1;
    *decision=AEON_IO_DENY;
    if (req->io_class!=AEON_IO_IPC && req->io_class!=AEON_IO_NETWORK_TX && req->io_class!=AEON_IO_NETWORK_RX) return -2;
    rc=aeon_submit_semantic(registry,req->request); if (rc!=0) return rc;
    *decision=AEON_IO_ALLOW; return 0;
}
