#include "aeon_cognitive_core.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
int main(void){
 aeon_hv_t a={{0}},b={{0}},o={{0}}; a.w[0]=1;b.w[0]=2; aeon_vsa_bind(&o,&a,&b); assert(o.w[0]==3); aeon_vsa_permute(&o,&a,65); assert(o.w[1]&2u); assert(aeon_vsa_hamming(&a,&b)==2);
 uint8_t q[]={0x8f,0x01}; int8_t u[4]; assert(aeon_q4_0_unpack(q,4,u)==0); assert(u[0]==7&&u[1]==0&&u[2]==-8&&u[3]==-8);
 float x[]={1,2}; float y[1]; assert(aeon_q4_0_gemv(q,x,1,2,1.0f,y)==0); assert(fabsf(y[0]-7.0f)<1e-6f);
 aeon_snn_t s; assert(aeon_snn_init(&s)==0); assert(aeon_snn_step(&s,5,1)>=0);
 aeon_inference_state_t i={0}; aeon_inference_update(&i,2,3,0.1f,0.5f); assert(i.free_energy>0&&i.confidence>0);
 puts("AEON Batch 63 cognitive compute core: PASS"); return 0;
}
