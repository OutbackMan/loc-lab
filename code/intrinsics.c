#if !defined(INTRINSICS_C)
#define INTRINSICS_C

#include "defs.h"

#if defined(ARCH_ARM)
  // IMPORTANT(Ryan): Assuming armv8, so neon is available.
  #include <arm_neon.h>
  #define WRITE_BARRIER() asm volatile("dmb ish" ::: "memory")
#else
  // IMPORTANT(Ryan): Assuming SSE2 available.
  #include <x86intrin.h>
  // IMPORTANT(Ryan): x86_64 guarantees order of memory accesses. 
  // This means no memory fence is required, only a compiler directive to not reorder instructions.
  #define WRITE_BARRIER() asm volatile("" ::: "memory")
#endif

INTERNAL inline u32
read_timestamp_counter(void)
{
  return __builtin_readcyclecounter();
}

// NOTE(Ryan): Using __atomic variants over __sync to support arm.
INTERNAL inline u64
atomic_exchange_u64(u64 volatile* prev, u64 new)
{
  return __atomic_exchange_n(prev, new, __ATOMIC_SEQ_CST);
}

INTERNAL inline u64
atomic_add_u64(u64 volatile* prev, u64 amount_to_add)
{
  return __atomic_add_fetch(prev, amount_to_add, __ATOMIC_SEQ_CST);
}

INTERNAL inline s32
round_r32_to_s32(r32 r32_val)
{
#if defined(ARCH_ARM)
  return vcvts_s32_f32(r32_val);
#else
  return _mm_cvtss_si32(_mm_set_ss(r32_val));
#endif
}

#endif
