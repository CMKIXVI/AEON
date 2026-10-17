#include "aeon_kernel_device.h"
#include <string.h>
int aeon_identity_device_register(aeon_identity_device_t *d){if(!d||d->registered)return -1;memset(d,0,sizeof(*d));if(aeon_process_gate_init(&d->process_gate)!=0)return -2;d->major=1;d->minor=0;d->registered=1;return 0;}
int aeon_identity_device_unregister(aeon_identity_device_t *d){if(!d||!d->registered)return -1;d->registered=0;return 0;}
int aeon_identity_device_ioctl(aeon_identity_device_t *d,const aeon_did_registry_t *registry,const aeon_ioctl_request_t *io,void *result)
{
    if(!d||!registry||!io||!result||!d->registered||!io->user_buffer)return -1;
    if(io->command==AEON_DEVICE_IOCTL_SUBMIT){aeon_ioctl_submit_t *req=(aeon_ioctl_submit_t *)io->user_buffer;aeon_io_decision_t decision;int rc;if(io->size<sizeof(*req))return -2;rc=aeon_process_gate_submit(&d->process_gate,registry,req,&decision);*(aeon_io_decision_t *)result=decision;return rc;}
    if(io->command==AEON_DEVICE_IOCTL_QUERY){aeon_ioctl_query_t *q=(aeon_ioctl_query_t *)io->user_buffer;if(io->size<sizeof(*q))return -3;return aeon_process_gate_query(&d->process_gate,q->process_id,q);} return -4;
}
