#ifndef __GF3D_COLLISION_H__
#define __GF3D_COLLISION_H__
#include "gf3d_physics.h"

Bool gf3d_collision_is_colliding(BoundingBox e,BoundingBox other);
Bool gf3d_collision_update_entity(Entity *e);


#endif
