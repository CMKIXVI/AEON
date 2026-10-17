#include "aeon_semantic_wire.h"
#include <string.h>
static int put_u16(uint8_t **p,uint8_t *e,uint16_t v){if((size_t)(e-*p)<2u)return -1;(*p)[0]=(uint8_t)(v>>8);(*p)[1]=(uint8_t)v;*p+=2;return 0;}
static int put_u32(uint8_t **p,uint8_t *e,uint32_t v){if((size_t)(e-*p)<4u)return -1;(*p)[0]=(uint8_t)(v>>24);(*p)[1]=(uint8_t)(v>>16);(*p)[2]=(uint8_t)(v>>8);(*p)[3]=(uint8_t)v;*p+=4;return 0;}
static int put_u64(uint8_t **p,uint8_t *e,uint64_t v){if((size_t)(e-*p)<8u)return -1;for(unsigned i=0;i<8;++i)(*p)[i]=(uint8_t)(v>>(56u-8u*i));*p+=8;return 0;}
static int put_bytes(uint8_t **p,uint8_t *e,const void *s,size_t n){if((size_t)(e-*p)<n)return -1;memcpy(*p,s,n);*p+=n;return 0;}
static int put_str16(uint8_t **p,uint8_t *e,const char *s,size_t max){size_t n=0;while(n<max&&s[n]!='\0')++n;if(n==0||n==max||n>UINT16_MAX)return -1;if(put_u16(p,e,(uint16_t)n)||put_bytes(p,e,s,n))return -1;return 0;}
static int get_u16(const uint8_t **p,const uint8_t *e,uint16_t *v){if((size_t)(e-*p)<2u)return -1;*v=(uint16_t)(((*p)[0]<<8)|(*p)[1]);*p+=2;return 0;}
static int get_u32(const uint8_t **p,const uint8_t *e,uint32_t *v){if((size_t)(e-*p)<4u)return -1;*v=((uint32_t)(*p)[0]<<24)|((uint32_t)(*p)[1]<<16)|((uint32_t)(*p)[2]<<8)|(*p)[3];*p+=4;return 0;}
static int get_u64(const uint8_t **p,const uint8_t *e,uint64_t *v){if((size_t)(e-*p)<8u)return -1;*v=0;for(unsigned i=0;i<8;++i)*v=(*v<<8)|(*p)[i];*p+=8;return 0;}
static int get_bytes(const uint8_t **p,const uint8_t *e,void *d,size_t n){if((size_t)(e-*p)<n)return -1;memcpy(d,*p,n);*p+=n;return 0;}
static int get_str16(const uint8_t **p,const uint8_t *e,char *d,size_t cap){uint16_t n;if(get_u16(p,e,&n)||n==0||(size_t)n>=cap)return -1;if(get_bytes(p,e,d,n))return -1;d[n]='\0';return 0;}
int aeon_semantic_wire_encode(const aeon_semantic_frame_t *f,uint8_t *out,size_t cap,size_t *used)
{
    uint8_t *p,*e;if(!f||!out||!used||aeon_semantic_frame_validate(f)!=0)return -1;p=out;e=out+cap;
    if(put_u32(&p,e,AEON_SEMANTIC_WIRE_MAGIC)||put_u16(&p,e,AEON_SEMANTIC_WIRE_VERSION)||put_u16(&p,e,f->header.flags)||put_u32(&p,e,f->header.total_size)||put_bytes(&p,e,f->header.digest,32)||put_str16(&p,e,f->subject,sizeof(f->subject))||put_str16(&p,e,f->predicate,sizeof(f->predicate))||put_str16(&p,e,f->object,sizeof(f->object))||put_bytes(&p,e,f->auth.public_key,32)||put_bytes(&p,e,f->auth.signature,64)||put_bytes(&p,e,f->auth.vc_hash,32)||put_str16(&p,e,f->auth.did,sizeof(f->auth.did))||put_bytes(&p,e,&f->auth.human_verified,1)||put_u64(&p,e,f->friction.challenge)||put_bytes(&p,e,f->friction.context_hash,32)||put_bytes(&p,e,&f->friction.issued,1)||put_bytes(&p,e,&f->friction.satisfied,1)||put_u64(&p,e,f->attestation.challenge)||put_bytes(&p,e,f->attestation.context_hash,32)||put_bytes(&p,e,f->attestation.verifier_signature,64)||put_bytes(&p,e,f->attestation.verifier_public_key,32)||put_bytes(&p,e,&f->attestation.valid,1)||put_str16(&p,e,f->ipfs_cid,sizeof(f->ipfs_cid)))return -2;
    *used=(size_t)(p-out);return 0;
}
int aeon_semantic_wire_decode(const uint8_t *in,size_t len,aeon_semantic_frame_t *f)
{
    const uint8_t *p,*e;uint32_t magic,total;uint16_t version;
    if(!in||!f)return -1;memset(f,0,sizeof(*f));p=in;e=in+len;
    if(get_u32(&p,e,&magic)||get_u16(&p,e,&version)||magic!=AEON_SEMANTIC_WIRE_MAGIC||version!=AEON_SEMANTIC_WIRE_VERSION||get_u16(&p,e,&f->header.flags)||get_u32(&p,e,&total)||total==0||get_bytes(&p,e,f->header.digest,32)||get_str16(&p,e,f->subject,sizeof(f->subject))||get_str16(&p,e,f->predicate,sizeof(f->predicate))||get_str16(&p,e,f->object,sizeof(f->object))||get_bytes(&p,e,f->auth.public_key,32)||get_bytes(&p,e,f->auth.signature,64)||get_bytes(&p,e,f->auth.vc_hash,32)||get_str16(&p,e,f->auth.did,sizeof(f->auth.did))||get_bytes(&p,e,&f->auth.human_verified,1)||get_u64(&p,e,&f->friction.challenge)||get_bytes(&p,e,f->friction.context_hash,32)||get_bytes(&p,e,&f->friction.issued,1)||get_bytes(&p,e,&f->friction.satisfied,1)||get_u64(&p,e,&f->attestation.challenge)||get_bytes(&p,e,f->attestation.context_hash,32)||get_bytes(&p,e,f->attestation.verifier_signature,64)||get_bytes(&p,e,f->attestation.verifier_public_key,32)||get_bytes(&p,e,&f->attestation.valid,1)||get_str16(&p,e,f->ipfs_cid,sizeof(f->ipfs_cid)))return -2;
    if(p!=e)return -3;f->header.magic=AEON_SEMANTIC_FRAME_MAGIC;f->header.version=AEON_SEMANTIC_FRAME_VERSION;f->header.total_size=total;f->header.subject_len=(uint32_t)strlen(f->subject);f->header.predicate_len=(uint32_t)strlen(f->predicate);f->header.object_len=(uint32_t)strlen(f->object);return aeon_semantic_frame_validate(f);
}
