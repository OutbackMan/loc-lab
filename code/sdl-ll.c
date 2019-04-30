#include "SDL.h"

#include "defs.h" 
#include "timed-block.c"

INTERNAL void DEBUGGER_BREAKPOINT_MARKER(void) {TIMED_BLOCK();}
  
TimedBlockRecord translation_unit_timed_block_records[__COUNTER__];

int
main(int argc, __attribute__ ((unused)) char* argv[argc + 1])
{
  DEBUGGER_BREAKPOINT_MARKER();

  u32 hit_count = 0;
  u32 cycle_count = 0;
  RESET_AND_READ_TIMED_BLOCK_RECORDS(hit_count, cycle_count) {
    // add fflush()
    SDL_Log("hits: %d, cycles: %d\n", hit_count, cycle_count);
  }

  if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Loc Lab", "Hello, world!", NULL) < 0) {
    printf("failed to open msg box %s\n", SDL_GetError()); 
  }

  return EXIT_SUCCESS;
}
