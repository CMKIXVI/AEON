#include "aeon_transformer_compute.h"
#include <math.h>
#include <stddef.h>

int aeon_softmax(float *x, size_t n) {
    if (!x || !n) return -1;
    float m=x[0], s=0.0f;
    for(size_t i=1;i<n;i++) if(x[i]>m) m=x[i];
    for(size_t i=0;i<n;i++){ x[i]=expf(x[i]-m); s+=x[i]; }
    if(!(s>0.0f) || !isfinite(s)) return -2;
    for(size_t i=0;i<n;i++) x[i]/=s;
    return 0;
}

int aeon_rmsnorm(const float *x,float *out,size_t n,float epsilon){
    if(!x||!out||!n||epsilon<0.0f)return -1;
    float ss=0.0f; for(size_t i=0;i<n;i++) ss+=x[i]*x[i];
    float inv=1.0f/sqrtf(ss/(float)n+epsilon);
    for(size_t i=0;i<n;i++) out[i]=x[i]*inv;
    return isfinite(inv)?0:-2;
}

int aeon_rope(float *q,float *k,size_t heads,size_t head_dim,size_t position){
    if(!q||!k||!heads||!head_dim||head_dim%2u)return -1;
    for(size_t h=0;h<heads;h++) for(size_t j=0;j<head_dim;j+=2){
        float theta=(float)position*powf(10000.0f,-(float)j/(float)head_dim);
        float c=cosf(theta),s=sinf(theta);
        size_t b=h*head_dim+j;
        float q0=q[b],q1=q[b+1],k0=k[b],k1=k[b+1];
        q[b]=q0*c-q1*s; q[b+1]=q0*s+q1*c;
        k[b]=k0*c-k1*s; k[b+1]=k0*s+k1*c;
    }
    return 0;
}

int aeon_attention(const float *q,const float *k,const float *v,size_t tokens,size_t dim,float scale,float *out){
    if(!q||!k||!v||!out||!tokens||!dim||scale<=0.0f)return -1;
    float *scores=(float *)__builtin_alloca(tokens*sizeof(float));
    for(size_t t=0;t<tokens;t++){
        float a=0.0f; for(size_t d=0;d<dim;d++) a+=q[d]*k[t*dim+d];
        scores[t]=a*scale;
    }
    if(aeon_softmax(scores,tokens)!=0)return -2;
    for(size_t d=0;d<dim;d++){float a=0.0f;for(size_t t=0;t<tokens;t++)a+=scores[t]*v[t*dim+d];out[d]=a;}
    return 0;
}

int aeon_swiglu(const float *gate,const float *up,float *out,size_t n){
    if(!gate||!up||!out||!n)return -1;
    for(size_t i=0;i<n;i++){float g=gate[i];float silu=g/(1.0f+expf(-g));out[i]=silu*up[i];}
    return 0;
}
