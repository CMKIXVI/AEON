#include "aeon_system_embodiment.h"
#include <math.h>
#include <string.h>
int aeon_detect_cpu_caps(aeon_cpu_caps_t *c){if(!c)return-1;memset(c,0,sizeof(*c));
#if defined(__x86_64__)||defined(_M_X64)
c->arch=AEON_ARCH_X86_64;c->simd_bits=128;
#if defined(__AVX2__) c->avx2=1;c->simd_bits=256; #endif
#if defined(__AVX512F__) c->avx512f=1;c->simd_bits=512; #endif
#if defined(__AVX512VNNI__) c->vnni=1; #endif
#elif defined(__aarch64__)||defined(_M_ARM64)
c->arch=AEON_ARCH_ARM64;c->simd_bits=128;c->neon=1;
#endif
return 0;}
int aeon_validate_memory_region(const aeon_memory_region_t*r){if(!r||!r->bytes||!r->alignment)return-1;return (r->alignment&(r->alignment-1u))?-1:0;}
int aeon_sensor_push(aeon_sensor_frame_t*f,uint32_t k,uint64_t t,float v){if(!f||!isfinite(v))return-1;size_t s=f->count<AEON_SENSOR_MAX?f->count++:0;f->samples[s]=(aeon_sensor_sample_t){t,v,k,1};return 0;}
int aeon_tool_register(aeon_tool_bus_t*b,uint32_t id,uint32_t cap,aeon_tool_handler_t h,void*ctx){if(!b||!h||b->count>=AEON_TOOL_MAX)return-1;for(size_t i=0;i<b->count;i++)if(b->entries[i].id==id)return-1;b->entries[b->count++]=(aeon_tool_entry_t){id,cap,h,ctx};return 0;}
int aeon_tool_invoke(const aeon_tool_bus_t*b,uint32_t id,uint32_t cap,const void*i,size_t is,void*o,size_t oc,size_t*os){if(!b||!os)return-1;for(size_t n=0;n<b->count;n++){const aeon_tool_entry_t*e=&b->entries[n];if(e->id==id){if((e->required_capability&cap)!=e->required_capability)return-2;return e->handler(i,is,o,oc,os,e->context);}}return-3;}
int aeon_action_gate(const aeon_actuator_policy_t*p,float r,aeon_action_command_t*c){if(!p||!c||!isfinite(r))return-1;c->command=0;c->approved=0;if(p->emergency_stop||!p->motors_enabled)return 0;if(!isfinite(p->max_command)||p->max_command<0)return-1;if(fabsf(r)>p->max_command)return 0;c->command=r;c->approved=1;return 0;}
