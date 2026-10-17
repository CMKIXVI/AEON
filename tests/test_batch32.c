#include "aeon_semantic_wire.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static void init_frame(aeon_semantic_frame_t *f)
{
    memset(f,0,sizeof(*f));f->header.magic=AEON_SEMANTIC_FRAME_MAGIC;f->header.version=AEON_SEMANTIC_FRAME_VERSION;
    snprintf(f->subject,sizeof(f->subject),"did:aeon:human:wire32");snprintf(f->predicate,sizeof(f->predicate),"aeon:mayTransmit");snprintf(f->object,sizeof(f->object),"network:semantic-triple");
    f->header.subject_len=(uint32_t)strlen(f->subject);f->header.predicate_len=(uint32_t)strlen(f->predicate);f->header.object_len=(uint32_t)strlen(f->object);
    f->header.total_size=(uint32_t)sizeof(f->header)+f->header.subject_len+f->header.predicate_len+f->header.object_len+3u+(uint32_t)sizeof(f->auth)+(uint32_t)sizeof(f->friction)+(uint32_t)sizeof(f->attestation)+(uint32_t)sizeof(f->ipfs_cid);
    for(size_t i=0;i<sizeof(f->header.digest);++i)f->header.digest[i]=(uint8_t)i;
    f->auth.public_key[0]=0x11;f->auth.signature[0]=0x22;f->auth.vc_hash[0]=0x33;f->auth.human_verified=1;snprintf(f->auth.did,sizeof(f->auth.did),"%s",f->subject);
    f->friction.challenge=UINT64_C(0x0102030405060708);f->friction.context_hash[0]=0x44;f->friction.issued=1;f->friction.satisfied=1;
    f->attestation.challenge=f->friction.challenge;f->attestation.context_hash[0]=0x44;f->attestation.verifier_signature[0]=0x55;f->attestation.verifier_public_key[0]=0x66;f->attestation.valid=1;snprintf(f->ipfs_cid,sizeof(f->ipfs_cid),"bafywire32");
}
int main(void)
{
    aeon_semantic_frame_t a,b;uint8_t wire[4096];size_t used=0;init_frame(&a);
    assert(aeon_semantic_wire_encode(&a,wire,sizeof(wire),&used)==0);assert(used>0&&used<sizeof(wire));assert(aeon_semantic_wire_decode(wire,used,&b)==0);
    assert(b.header.magic==a.header.magic&&b.header.version==a.header.version);assert(strcmp(b.subject,a.subject)==0);assert(strcmp(b.predicate,a.predicate)==0);assert(strcmp(b.object,a.object)==0);assert(memcmp(b.header.digest,a.header.digest,32)==0);assert(b.friction.challenge==a.friction.challenge);assert(strcmp(b.auth.did,a.auth.did)==0);assert(strcmp(b.ipfs_cid,a.ipfs_cid)==0);
    wire[0]^=1u;assert(aeon_semantic_wire_decode(wire,used,&b)!=0);puts("AEON Batch 32 semantic wire codec test: PASS");return 0;
}
