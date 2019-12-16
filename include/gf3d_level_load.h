#ifndef __GF3D_LEVEL_LOAD_H__
#define __GF3D_LEVEL_LOAD_H__
#include "gf3d_sprite.h"

#include "gfc_list.h"
#include "simple_json.h";
#include "simple_json_string.h"
#include "simple_json_parse.h"
#include "simple_json_error.h"
#include "simple_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void gf3d_level_load_read_config(const char * filename);
void gf3d_level_load_sprite_init(SJson *sprites);
void gf3d_level_load_model_init();
void gf3d_level_load_music_init();

#endif
