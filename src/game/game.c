#include "game.h"

conf_t conf;
mtar_t tar;
mtar_header_t h;

void conf_init(const char* path) {
  JSON_Object* root = json_object(json_parse_file("conf.json"));
  JSON_Array* res = json_object_get_array(root, "res");
  conf.width = json_array_get_number(res, 0);
  conf.height = json_array_get_number(res, 1);
  conf.fullscreen = json_object_get_boolean(root, "fullscreen");
  conf.msaa = json_object_get_number(root, "msaa");
  conf.fov = json_object_get_number(root, "fov");
  conf.sens = json_object_get_number(root, "sens");
  log_info("Loaded config \"%s\".", path);
}

void conf_write(const char* path) {
  JSON_Value* rootv = json_value_init_object();
  JSON_Object* root = json_object(rootv);
  JSON_Value* resv = json_value_init_array();
  JSON_Array* res = json_array(resv);
  json_array_append_value(res, json_value_init_number(conf.width));
  json_array_append_value(res, json_value_init_number(conf.height));
  json_object_set_value(root, "res", resv);
  json_object_set_boolean(root, "fullscreen", conf.fullscreen);
  json_object_set_number(root, "msaa", conf.msaa);
  json_object_set_number(root, "fov", conf.fov);
  json_object_set_number(root, "sens", conf.sens);

  json_serialize_to_file_pretty(rootv, "conf.json");
  log_info("Wrote config \"%s\".", path);
}

void assets_init(const char* path) {
  if (mtar_open(&tar, path, "r")) {
    log_fatal("Could not load \"%s\".", path);
    exit(-1);
  }
  log_info("Loaded \"%s\".", path);
}

asset_t asset_load(const char* path) {
  asset_t asset;
  char base[64] = "res/";
  mtar_find(&tar, strcat(base, path), &h);
  asset.len = h.size;
  asset.data = calloc(1, h.size + 1);
  mtar_read_data(&tar, asset.data, h.size);
  return asset;
}