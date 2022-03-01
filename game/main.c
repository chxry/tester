#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "engine/gfx.h"
#include "engine/audio.h"
#include "engine/log.h"
#include "engine/core.h"
#include "game.h"
#include "world.h"
#include "ui.h"
#include "player.h"

int main() {
  log_init();
  conf_init("conf.json");
  assets_init("res.tar");
  lua_init();
  window_init(conf.width, conf.height, conf.fullscreen, "flop");
  ui_init(window, &ctx);
  audio_init(conf.volume);
  world_init();

  unsigned int skybox_tex = tex_load_cubemap((char* [6]){"tex/sky/right.jpg", "tex/sky/left.jpg", "tex/sky/top.jpg", "tex/sky/bottom.jpg", "tex/sky/front.jpg", "tex/sky/back.jpg"}, GL_RGB);
  unsigned int skybox_shader = shader_init("shaders/skybox.vert", "shaders/skybox.frag");
  basic_shader = shader_init("shaders/basic.vert", "shaders/basic.frag");
  debug_shader = shader_init("shaders/debug.vert", "shaders/debug.frag");

  bool quit = false;
  frame_delay = 1000 / conf.fps;
  while (!quit) {
    int frame_start = SDL_GetTicks();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {

      case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
          gbuffer_resize(e.window.data1, e.window.data2);
          conf.width = e.window.data1;
          conf.height = e.window.data2;
        }
        break;
      case SDL_QUIT:
        quit = true;
        break;
      }
      ui_processevent(&e);
      player_processevent(&e);
    }

    mat4 light_view, light_projection;
    gbuffer_render_shadows(light_view, light_projection, state.world_lightdir);
    world_render_shadows(light_view, light_projection);

    mat4 view, projection;
    player_movement(&view);
    glm_perspective(glm_rad(conf.fov), (float)conf.width / (float)conf.height, 0.1, 100.0, projection);
    glViewport(0, 0, conf.width, conf.height);
    glClearColor(0, 0, 0, 1);
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world_render(view, projection);

    glDepthFunc(GL_LEQUAL);
    shader_use(skybox_shader);
    tex_use_cubemap(skybox_tex);
    mat4 skybox_view = GLM_MAT4_ZERO;
    mat3 view3;
    glm_mat4_pick3(view, view3);
    glm_mat4_ins3(view3, skybox_view);
    shader_set_mat4(skybox_shader, "view", skybox_view);
    shader_set_mat4(skybox_shader, "projection", projection);
    mesh_render(get_mesh("mesh/sky.obj", pos));
    glDepthFunc(GL_LESS);

    gbuffer_render(light_view, light_projection, state.world_lightdir);
    ui_render(window);
    SDL_GL_SwapWindow(window);
    int frame_time = SDL_GetTicks() - frame_start;
    if (frame_delay > frame_time) {
      SDL_Delay(frame_delay - frame_time);
    }
  }

  return 0;
}