#if !defined(DEFS_H)
#define DEFS_H

#define TO_STRING(val) #val
#define STRINGIFY(val) TO_STRING(val)

#if defined(__clang__)
  #define CLANG 1
  #define COMPILER "llvm-"STRINGIFY(__clang_major__)"." \
    STRINGIFY(__clang_minor__)"."STRINGIFY(__clang_patchlevel__)
#else
  #error "Unsupported compiler. Use clang."
#endif

// NOTE(Ryan): The main archictectures in use are x86_64 and armv8.
#if defined(__x86_64__)
  #define X86_64 1
#elif defined(__ARM_ARCH_8__)
  #define ARM 1
#else
  #error "Unsupported architecture. Target x86_64, or armv8."
#endif

#if defined(__ANDROID__)
  #define ANDROID 1
#elif defined(__linux__)
  #define GNU_LINUX 1
#elif defined(__APPLE__)
  #include <TargetConditionals.h>
  #if TARGET_OS_PHONE
    #define IPHONEOS 1
  #endif
  #define MACOS 1
#elif defined(_WIN64)
  #define WINDOWS 1
#else
  #error "Unsupported OS. Use gnu/linux, window, macos, iphoneos or android."
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define GLOBAL static
#define INTERNAL static
#define PERSIST static

#define KILOBYTES(n) ((n) * 1024UL)
#define MEGABYTES(n) (KILOBYTES(n) * 1024UL)
#define GIGABYTES(n) (MEGABYTES(n) * 1024UL)

#define MIN(val1, val2) ((val1) < (val2) ? (val1) : (val2))
#define MAX(val1, val2) ((val1) > (val2) ? (val1) : (val2))
#define CLAMP_MIN(val, min) MAX(val, min)
#define CLAMP_MAX(val, max) MIN(val, max)

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

// NOTE(Ryan): The sizes of these are irrelevent of the target architecture as they are implemented following the IEEE 754 standard.
typedef float r32;
typedef double r64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif
