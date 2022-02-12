#pragma once
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cglm/cglm.h>
#include <log.h>

#include "util.h"

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
} mesh_t;

void window_init(char* title, int w, int h, bool fullscreen);
void window_loop();
void window_destroy();
unsigned int shader_init(const char* vert_path, const char* frag_path);
void shader_set_mat4(unsigned int shader, const char* name, mat4 val);
void shader_use(unsigned int shader);
mesh_t mesh_init();
void mesh_render(mesh_t mesh);
