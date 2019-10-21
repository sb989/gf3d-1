#ifndef __GF3D_COLLISION_H__
#define __GF3D_COLLISION_H__
#include "gf3d_physics.h"
#include "gfc_list.h"
Bool gf3d_collision_is_colliding(Entity * e1,Entity * other1);
void gf3d_collision_update_entity(Entity *e);
void gf3d_collision_print_collision_box(BoundingBox b);
Vector3D gf3d_face_collision_detection(Entity * e, Entity * other);
#endif
