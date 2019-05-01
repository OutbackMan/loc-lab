#if !defined(LOG_C)
#define LOG_C

#if defined(ANDROID)
  #include <android/log.h>
#endif

#include "defs.h"

INTERNAL void
log(char const* fmt, ...)
{
  va_list args; 
  va_start(args, fmt);
#if defined(ANDROID)
  // TODO(Ryan): Does this enter a new line automatically?
  __android_log_vprint(ANDROID_LOG_INFO, "LL", fmt, args);
#elif defined(IPHONEOS)
  // TODO(Ryan): Does this enter a new line automatically?
  NSLogv(@(fmt), args);
#else
  printf("[LL INFO]");
  vprintf(fmt, args);
  printf("\n");
  fflush(stdout);
#endif
  va_end(args);
}

#endif
