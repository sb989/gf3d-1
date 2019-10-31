#include <math.h>
#include <string.h>
#include "gf3d_physics.h"
#include "gf3d_vgraphics.h"
#include "simple_logger.h"

float gf3d_physics_current_time()
{
  return clock() - t;
}

void gf3d_physics_set_time()
{
  t = clock();
}
void gf3d_physics_teleport(Entity *e,Vector3D dist)
{
  Matrix4 temp;
  gfc_matrix_copy(temp,*(e->entityMat));
  gfc_matrix_translate(temp,dist);
  gfc_matrix_copy(*(e->entityMat),temp);
  e->position.x = temp[3][0];
  e->position.y = temp[3][1];
  e->position.z = temp[3][2];
}
void gf3d_physics_attack_enemy(Entity *e,Vector3D push)
{
  vector3d_negate(e->velocity,e->velocity);
}
void gf3d_physics_collision_push_back(Entity *e,Vector3D push)
{

  //gf3d_physics_set_velocity(e, vector3d(0,0,0));

  //vector3d_scale(vel,vel,1/10);
  //vector3d_negate(e->velocity,vel);
  Vector3D dist;
  if(push.z != 0)
  {
    vector3d_scale(dist,push,.007);
    e->velocity.z = 0;
  }
  else if(push.x!=0)
  {
    vector3d_scale(dist,push,.2);
    e->velocity.x = 0;
  }
  else
  {
    vector3d_scale(dist,push,.2);
    e->velocity.y = 0;
  }
  //vector3d_scale(dist,-vel,.1095);
  Matrix4 temp;
  gfc_matrix_copy(temp,*(e->entityMat));
  gfc_matrix_translate(temp,dist);
  gfc_matrix_copy(*(e->entityMat),temp);
  e->position.x = temp[3][0];
  e->position.y = temp[3][1];
  e->position.z = temp[3][2];
  if(strcmp(e->name,"mario")==0)
  {
    //slog("mario");
    //gf3d_vgraphics_move_camera(dist);
  }

}
void gf3d_physics_update(Entity * e)
{
  //Vector3D pos = e->position;
  Matrix4 mat;
  Vector3D vel = e->velocity;
  Vector3D acel = e->acceleration;
  double lastUpdate = e->lastUpdate;
  Vector3D dist;
  float deltaT = gf3d_physics_current_time()-lastUpdate;
  if(vel.x != 0 && vel.y != 0 && vel.z != 0)
  {
    e->lastVel = vel;
  }
  deltaT = deltaT/100000;
  //deltaT = deltaT/CLOCKS_PER_SEC;
  vector3d_scale(dist,vel,deltaT);
  //slog("%f",deltaT);
  Matrix4 temp;
  gfc_matrix_copy(temp,*(e->entityMat));
  gfc_matrix_translate(temp,dist);

  Vector3D deltaV;
  vector3d_scale(deltaV,acel,deltaT);
  vector3d_add(e->velocity,vel,deltaV);
  e->lastUpdate = gf3d_physics_current_time();
  gfc_matrix_copy(*(e->entityMat),temp);
  gfc_matrix_copy(mat,*(e->entityMat));
  e->position.x = mat[3][0];
  e->position.y = mat[3][1];
  e->position.z = mat[3][2];
  if(strcmp(e->name,"mario")==0)
  {
    Vector3D cam,camPos;
    cam = vector3d(0,0,0);
    camPos = gf3d_vgraphics_camera_position();
    if(camPos.y-mat[3][1]>20)
      cam.y = dist.y;
    if(camPos.z-mat[3][2] >10)
      cam.z = dist.z;
    cam.x = dist.x;
    gf3d_vgraphics_move_camera(cam);
  }
  //slog("%f",e->position.z);
  //slog("matrix");
  //gfc_matrix_slog(*(e->entityMat));


  //slog("update entity updatetime");

}
void gf3d_physics_set_velocity(Entity *e, Vector3D velocity)
{
  e->velocity = velocity;
}

void gf3d_physics_add_velocity(Entity *e,Vector3D velocity)
{
  vector3d_add(e->velocity,e->velocity,velocity);
}

void gf3d_physics_set_acceleration(Entity *e, Vector3D acceleration)
{
  e->acceleration = acceleration;
}
