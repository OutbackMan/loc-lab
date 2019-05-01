#include "SDL.h"
#include "SDL_opengl.h"

#include "defs.h" 
#include "log.c"

INTERNAL void DEBUGGER_BREAKPOINT_MARKER(void) {}

GLOBAL bool want_to_run = false;

#define BYTES_PER_PIXEL 4U
typedef struct {
  void* memory;
  u32 width;
  u32 height;
  u32 pitch;
} LLPixelBuffer;

int
main(int argc, __attribute__ ((unused)) char* argv[argc + 1])
{
  DEBUGGER_BREAKPOINT_MARKER();

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0) {
    log("Unable to initialize sdl: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  u32 window_width = 1920;
  u32 window_height = 1080;
  SDL_Window* window = SDL_CreateWindow(
                                        "Loc Lab", 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        window_width, window_height, 
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
                                       );
  if (window == NULL) {
    log("Unable to create sdl window: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  want_to_run = true;
  while (want_to_run) {
    SDL_Event event = {0};
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_QUIT: {
          want_to_run = false; 
         } break;
        case SDL_WINDOWEVENT: {
          if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
            want_to_run = false;             
          }
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            window_width = event.window.data1; 
            window_height = event.window.data2; 
          }
        } break;
      } 
    } // NOTE(Ryan): End event loop

    opengl_display_ll_pixel_buffer();

  }

  return EXIT_SUCCESS;
}
