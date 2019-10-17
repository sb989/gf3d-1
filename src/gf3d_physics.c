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

void gf3d_physics_collision_push_back(Entity *e)
{
  Vector3D vel = e->velocity;
  gf3d_physics_set_velocity(e, vector3d(0,0,0));
  Vector3D dist;
  vector3d_scale(dist,-vel,.001);
  Matrix4 temp;
  gfc_matrix_copy(temp,*(e->entityMat));
  gfc_matrix_translate(temp,dist);
  gfc_matrix_copy(*(e->entityMat),temp);
  e->position.x = temp[3][0];
  e->position.y = temp[3][1];
  e->position.z = temp[3][2];

}
void gf3d_physics_update(Entity * e)
{
  //Vector3D pos = e->position;
  Matrix4 mat;

  Vector3D vel = e->velocity;
  Vector3D acel = e->acceleration;
  float lastUpdate = e->lastUpdate;
  Vector3D dist;
  float deltaT = gf3d_physics_current_time()-lastUpdate;
  deltaT = deltaT/200000;
  vector3d_scale(dist,vel,deltaT);
  //slog("%f",deltaT);
  Matrix4 temp;
  gfc_matrix_copy(temp,*(e->entityMat));
  gfc_matrix_translate(temp,dist);
  //slog("%f,%f",dist.z,e->entityBoundingBoxes[0].boundingZ1);

  //slog("%f,%f",dist.z,e->entityBoundingBoxes[0].boundingZ1);
  //gfc_matrix_translate(*(e->entityMat),dist);
  //vector3d_add(e->position,pos,dist);
  //slog("moved the entity");
  Vector3D deltaV;
  vector3d_scale(deltaV,acel,deltaT);
  vector3d_add(e->velocity,vel,deltaV);
  e->lastUpdate = gf3d_physics_current_time();
  gfc_matrix_copy(*(e->entityMat),temp);
  gfc_matrix_copy(mat,*(e->entityMat));
  e->position.x = mat[3][0];
  e->position.y = mat[3][1];
  e->position.z = mat[3][2];

  //slog("%f",e->position.z);
  //slog("matrix");
  //gfc_matrix_slog(*(e->entityMat));


  //slog("update entity updatetime");

}
void gf3d_physics_set_velocity(Entity *e, Vector3D velocity)
{
  e->velocity = velocity;
}


void gf3d_physics_set_acceleration(Entity *e, Vector3D acceleration)
{
  e->acceleration = acceleration;
}
