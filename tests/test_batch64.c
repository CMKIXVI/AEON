#include "aeon_transformer_compute.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void){
 float x[]={3.0f,4.0f},y[2]; assert(aeon_rmsnorm(x,y,2,0.0f)==0); assert(fabsf(y[0]-0.6f)<1e-5f); assert(fabsf(y[1]-0.8f)<1e-5f);
 float p[]={1,0,0,1}; assert(aeon_rope(p,p,1,4,0)==0); assert(fabsf(p[0]-1)<1e-6f);
 float q[]={1,0},k[]={1,0,0,1},v[]={2,3,4,5},o[2]; assert(aeon_attention(q,k,v,2,2,1.0f,o)==0); assert(o[0]>2.0f&&o[0]<4.1f);
 float g[]={0,1},u[]={2,3},z[2]; assert(aeon_swiglu(g,u,z,2)==0); assert(z[0]==0.0f&&z[1]>1.5f);
 float s[]={1,2,3}; assert(aeon_softmax(s,3)==0); assert(fabsf(s[0]+s[1]+s[2]-1.0f)<1e-6f);
 puts("AEON Batch 64 model compute: PASS"); return 0;
}
