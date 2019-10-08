#include <math.h>
#include <gf3d_physics.h>
#include "simple_logger.h"

float gf3d_physics_current_time()
{
  return clock() - t;
}

void gf3d_physics_set_time(){
  t = clock();
}


void gf3d_physics_update(Entity * e)
{
  //Vector3D pos = e->position;
  Vector3D vel = e->velocity;
  Vector3D acel = e->acceleration;
  float lastUpdate = e->lastUpdate;
  Vector3D dist;
  float deltaT = gf3d_physics_current_time()-lastUpdate;
  deltaT = deltaT/1000;
  vector3d_scale(dist,vel,deltaT);
  slog("%f",deltaT);
  gfc_matrix_translate(*(e->entityMat),dist);
  //vector3d_add(e->position,pos,dist);
  Vector3D deltaV;
  vector3d_scale(deltaV,acel,deltaT);
  vector3d_add(e->velocity,vel,deltaV);
  e->lastUpdate = gf3d_physics_current_time();

}
void gf3d_physics_set_velocity(Entity *e, Vector3D velocity)
{
  e->velocity = velocity;
}


void gf3d_physics_set_acceleration(Entity *e, Vector3D acceleration)
{
  e->acceleration = acceleration;
}
