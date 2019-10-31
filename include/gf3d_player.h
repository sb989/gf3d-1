#ifndef __GF3D_PLAYER_H__
#define __GF3D_PLAYER_H__
#include <math.h>
#include "gf3d_entity.h"
#include "gf3d_vgraphics.h"
#include "gf3d_physics.h"
#include "simple_logger.h"
#include "gf3d_mesh.h"

Entity * gf3d_player_init();
void gf3d_player_think();
void gf3d_player_update(Entity * e);
void gf3d_player_update_frame();
void gf3d_player_draw(Uint32 bufferFrame,VkCommandBuffer commandBuffer);


#endif
