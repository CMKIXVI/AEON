#include "aeon_syscall.h"
#include <string.h>
int aeon_submit_semantic(const aeon_did_registry_t *registry,const aeon_submit_request_t *req)
{
    const aeon_semantic_transmission_t *tx; int rc;
    if(!registry||!req||req->abi_version!=AEON_IDENTITY_ABI_VERSION)return -1;
    tx=&req->transmission;
    if((req->flags&AEON_SUBMIT_REQUIRE_HUMAN)&&!tx->friction_verified)return -2;
    if((req->flags&AEON_SUBMIT_REQUIRE_DID)&&!tx->auth.human_verified)return -3;
    if((req->flags&AEON_SUBMIT_REQUIRE_IPFS)&&tx->ipfs_cid[0]=='\0')return -4;
    if(req->flags&AEON_SUBMIT_REQUIRE_VC){uint8_t zero[AEON_VC_HASH_BYTES]={0};if(memcmp(tx->auth.vc_hash,zero,sizeof(zero))==0)return -5;}
    rc=aeon_semantic_verify_transmission(registry,tx);return rc==0?0:-6;
}
