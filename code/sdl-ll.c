#include "SDL.h"
#include "SDL_opengl.h"

#include "defs.h" 
#include "log.c"
#include "intrinsics.c"

INTERNAL void DEBUGGER_BREAKPOINT_MARKER(void) {}

GLOBAL bool want_to_run = false;

#define BYTES_PER_PIXEL 4U
typedef struct {
  u32* memory;
  u32 width;
  u32 height;
  u32 pitch;
} LLPixelBuffer;

typedef struct {
  s32 min_x, min_y;
  s32 max_x, max_y;
} Rectangle2i;

GLOBAL GLunit opengl_reserved_blit_texture = 0;

INTERNAL void
opengl_display_pixel_buffer(LLPixelBuffer* pixel_buffer, Rectangle2i draw_region, GLuint blit_texture)
{
  u32 draw_width = draw_region.max_x - draw_region.min_x;
  u32 draw_height = draw_region.max_y - draw_region.min_y;

  glViewport(draw_region.min_x, draw_region.min_y, draw_width, draw_height);

  glDisable(GL_SCISSOR_TEST); // ??
  glEnable(GL_BLEND); 

  glBindTexture(GL_TEXTURE_2D, blit_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pixel_buffer->width, pixel_buffer->height, 0, GL_RGBA, GL_UINT_8_8_8_8, pixel_buffer->memory);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);    
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glEnable(GL_TEXTURE_2D);

  glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glBegin(GL_TRIANGLES);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);

  glEnd();
}

Rectangle2i
compute_drawable_region_from_aspect_ratio(u32 render_width, u32 render_height, u32 window_width, u32 window_height)
{
  Rectangle2i result = {0};

  // NOTE(Ryan): Better to only have parameter checking on public calls, but do everywhere when developing
  if ((render_width > 0) &&
      (render_height > 0) &&
      (window_width > 0) &&
      (window_height > 0)) {

    r32 optimal_window_width = (r32)window_height * ((r32)render_width / (r32)render_height);
    r32 optimal_window_height = (r32)window_width * ((r32)render_height / (r32)render_width);

    if (optimal_window_width > (r32)window_width) {
      result.min_x = 0;
      result.max_x = window_width; 

      r32 empty_vspace = (r32)window_height - optimal_window_height;
      s32 half_empty_vspace = round_r32_to_s32(0.5f * empty_vspace);
      s32 rounded_empty_vspace = round_r32_to_32(empty_vspace);

      result.min_y = half_emtpy_vspace;
      result.max_y = result.y + rounded_empty_vspace;
    } else {
      result.min_y = 0;
      result.max_x = window_height; 

      r32 empty_hspace = (r32)window_width - optimal_window_width;
      s32 half_empty_hspace = round_r32_to_s32(0.5f * empty_hspace);
      s32 rounded_empty_hspace = round_r32_to_s32(empty_hspace);

      result.min_x = half_empty_hspace;
      result.max_x = result.x + rounded_empty_hspace;
    }
  }
  
  return result;
}

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

  SDL_GLContext context = SDL_GL_CreateContext(window);
    if (SDL_GL_SetSwapInterval(-1) < 0) {
      log("Unable to enable adaptive vsync for sdl opengl context: %s", SDL_GetError());
      if (SDL_GL_SetSwapInterval(1) < 0) {
        log("Unable to enable vsync for sdl opengl context: %s", SDL_GetError());
        // TODO(Ryan): Investigate if this occurs frequently enough to merit a fallback.
        return EXIT_FAILURE;
      }
    }

  // enable opengl here

  glGenTextures(1, &opengl_reserved_blit_texture);


  LLPixelBuffer pixel_buffer = {0};
  pixel_buffer.width = window_width;
  pixel_buffer.height = window_height;
  pixel_buffer.pitch = pixel_buffer.width * BYTES_PER_PIXEL;
  pixel_buffer.memory = calloc(pixel_buffer.width * pixel_buffer.height, BYTES_PER_PIXEL);
  if (pixel_buffer.memory == NULL) {
    log("Unable to allocate memory for ll pixel buffer: %s", strerror(errno));
    return EXIT_FAILURE;
  }

  Rectangle2i drawable_region = {0};

  u8* pixel_buffer_row = (u8 *)pixel_buffer.memory;
  for (u32 y = 0; y < pixel_buffer.height; ++y) {
    u32* pixel = (u32 *)pixel_buffer_row;
    for (u32 x = 0; x < pixel_buffer.width; ++x) {
      *pixel++ = 0x0000FFFF; 
    }
    pixel_buffer_row += pixel_buffer.pitch;
  }

  want_to_run = true;
  while (want_to_run) {
    SDL_Event event = {0};

    drawable_region = compute_drawable_region_from_aspect_ratio(pixel_buffer->width, pixel_buffer->height, window_width, window_height);

    // mouse_x = Clamp01MapToRange(draw_region.min_x, )

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
    SDL_GL_SwapWindow(window);
  }

  return EXIT_SUCCESS;
}
