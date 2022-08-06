#pragma once
#include <parson/parson.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <iconfonts/IconsFontAwesome4.h>

#include "util/ui.h"
#include "ecs/world.h"

typedef struct {
  int entity;
  char* path;
} luascript_t;

extern component_t luascript;