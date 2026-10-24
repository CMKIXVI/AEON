#include "aeon_cognitive_core.h"
#include <string.h>
#include <math.h>

void aeon_vsa_zero(aeon_hv_t *v) { if (v) memset(v, 0, sizeof(*v)); }
void aeon_vsa_bind(aeon_hv_t *out, const aeon_hv_t *a, const aeon_hv_t *b) {
    if (!out || !a || !b) return;
    for (size_t i=0;i<AEON_VSA_WORDS;i++) out->w[i]=a->w[i]^b->w[i];
}
void aeon_vsa_bundle(aeon_hv_t *out, const aeon_hv_t *const *items, size_t count) {
    if (!out) return; aeon_vsa_zero(out); if (!items || !count) return;
    for (size_t bit=0;bit<AEON_VSA_BITS;bit++) { size_t ones=0;
        for (size_t j=0;j<count;j++) if (items[j] && ((items[j]->w[bit>>6]>>(bit&63u))&1u)) ones++;
        if (ones*2u>=count) out->w[bit>>6]|=UINT64_C(1)<<(bit&63u);
    }
}
void aeon_vsa_permute(aeon_hv_t *out, const aeon_hv_t *in, unsigned shift) {
    if (!out || !in) return; shift%=AEON_VSA_BITS; if (!shift){*out=*in;return;}
    aeon_hv_t t; aeon_vsa_zero(&t);
    for (size_t bit=0;bit<AEON_VSA_BITS;bit++) if ((in->w[bit>>6]>>(bit&63u))&1u) {
        size_t dst=(bit+shift)%AEON_VSA_BITS; t.w[dst>>6]|=UINT64_C(1)<<(dst&63u);
    }
    *out=t;
}
unsigned aeon_vsa_hamming(const aeon_hv_t *a, const aeon_hv_t *b) {
    if (!a || !b) return 0; unsigned n=0; for(size_t i=0;i<AEON_VSA_WORDS;i++) n+=(unsigned)__builtin_popcountll(a->w[i]^b->w[i]); return n;
}
int aeon_snn_init(aeon_snn_t *s) {
    if(!s)return -1; memset(s,0,sizeof(*s));
    for(size_t i=0;i<AEON_SNN_NODES;i++){s->node[i].v=-65.0f;s->node[i].u=-13.0f;s->node[i].a=0.02f;s->node[i].b=0.2f;s->node[i].c=-65.0f;s->node[i].d=8.0f;}
    return 0;
}
int aeon_snn_step(aeon_snn_t *s,float input,float reward){
    if(!s)return -1; size_t spikes=0;
    s->dopamine += 0.05f*(reward-s->dopamine); s->norepinephrine += 0.05f*(fabsf(input)-s->norepinephrine);
    for(size_t i=0;i<AEON_SNN_NODES;i++){aeon_snn_node_t*n=&s->node[i]; float I=input+(float)(i%AEON_SNN_SYNAPSES)*0.01f;
        float dv=0.04f*n->v*n->v+5.0f*n->v+140.0f-n->u+I; float du=n->a*(n->b*n->v-n->u); n->v+=dv*0.01f; n->u+=du*0.01f;
        if(n->v>=30.0f){n->v=n->c;n->u+=n->d;n->spikes++;spikes++;}
    }
    s->branching_ratio=(float)spikes/(float)AEON_SNN_NODES; return (int)spikes;
}
void aeon_inference_update(aeon_inference_state_t*s,float prediction,float observation,float uncertainty,float goal_error){
    if(!s)return; float e=observation-prediction; s->prediction_error=e; s->free_energy=0.5f*e*e+fmaxf(uncertainty,0.0f); s->expected_free_energy=s->free_energy+fmaxf(goal_error,0.0f); s->confidence=1.0f/(1.0f+fabsf(e)+fmaxf(uncertainty,0.0f));
}
int aeon_q4_0_unpack(const uint8_t*packed,size_t count,int8_t*out){
    if(!packed||!out)return -1; for(size_t i=0;i<count;i++){uint8_t v=packed[i>>1]; out[i]=(int8_t)(((i&1u)?(v>>4):(v&0x0fu))-8); } return 0;
}
int aeon_q4_0_gemv(const uint8_t*w,const float*x,size_t rows,size_t cols,float scale,float*out){
    if(!w||!x||!out||!cols||!rows)return -1; for(size_t r=0;r<rows;r++){float acc=0; for(size_t c=0;c<cols;c++){uint8_t v=w[(r*cols+c)>>1]; int q=(int)(((c&1u)?v>>4:v&15u))-8; acc+=(float)q*x[c]*scale;} out[r]=acc;} return 0;
}
