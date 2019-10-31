#ifndef __GF3D_PHYSICS_H__
#define __GF3D_PHYSICS_H__
#include <time.h>
#include "gfc_matrix.h"
#include "gf3d_entity.h"



static double t;
void gf3d_physics_set_time();
float gf3d_physics_current_time();

void gf3d_physics_update(Entity *e);
void gf3d_physics_collision_push_back(Entity *e,Vector3D push);
void gf3d_physics_set_velocity(Entity *e,Vector3D velocity);
void gf3d_physics_set_acceleration(Entity *e, Vector3D acceleration);
void gf3d_physics_add_velocity(Entity *e,Vector3D velocity);

#endif
