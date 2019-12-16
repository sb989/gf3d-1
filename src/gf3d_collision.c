#include "gf3d_collision.h"
#include "gf3d_entity.h"
#include "gf3d_mesh.h"
#include "simple_logger.h"
#include "gfc_list.h"
Bool gf3d_collision_is_colliding(Entity * e1,Entity * other1)
{

//  gf3d_collision_print_collision_box(e);
  //slog("mario^^^");
//  gf3d_collision_print_collision_box(other);
//  slog("other^^^");
  BoundingBox e,other;
  e = e1->entityBoundingBoxes;
  other = other1->entityBoundingBoxes;
  if(e.boundingX1 <= other.boundingX2 && e.boundingX2 >= other.boundingX1)
  {

    if(e.boundingY1-5 <= other.boundingY2 && e.boundingY2-.5 >= other.boundingY1)
    {

      if(e.boundingZ1 <= other.boundingZ2 && e.boundingZ2 >= other.boundingZ1)
      {
        return true;
      }
    }
  }
  return false;

}

void gf3d_collision_update_entity(Entity *e)
{
  int i,j,count;
  Entity * other;
  //List * collisions;
  //collisions = gfc_list_new();
  //slog("collision update");
  if (e->isEnvironment2) return;
  else
  {
    for(i = 0; i < gf3d_entity_manager_get_size();i++)
    {
      other = gf3d_entity_manager_get_entity(i);
      if(e==other) continue;
      if(other->_inuse == 0) continue;
      if(other->state == ES_Dead) continue;

      //slog("%s,%s",e->name,other->name);
      else
      {
        //slog("%s,%s",e->name,other->name);
        if(gf3d_collision_is_colliding(e,other))
        {
          //  slog("colliding with %s",other->name);
          //  slog("%f<=%f && %f>=%f",e->entityBoundingBoxes.boundingY1,other->entityBoundingBoxes.boundingY2,e->entityBoundingBoxes.boundingY2,other->entityBoundingBoxes.boundingY1);
            //slog(gf3d_face_collision_detection(e,other));
            Vector3D push = gf3d_face_collision_detection(e,other);
            if(other->isEnemy && e->velocity.z<-.5)
            {
              gf3d_physics_attack_enemy(e,push);
              other->state = ES_Dead;
            }
            else if(other->isEnemy)
            {
              vector3d_scale(push,push,50);
              gf3d_physics_collision_push_back(e,push);
              //gf3d_physics_attack_enemy(e,push);
              e->health = e->health -2;
              if(e->health ==0 && strcmp(e->name,"mario")==0)
              {
                gf3d_level_load_read_config("config/gameover.json");
              }
            }
            else
              gf3d_physics_collision_push_back(e,push);

        }
      }
    }
  }

}

void gf3d_collision_print_collision_box(BoundingBox b)
{
  slog("min coords : [%f,%f,%f]",b.boundingX1,b.boundingY1,b.boundingZ1);
  slog("max coords : [%f,%f,%f]",b.boundingX2,b.boundingY2,b.boundingZ2);
}

Vector3D gf3d_face_collision_detection(Entity * e, Entity * other)
{
  BoundingBox b1 = e->entityBoundingBoxes;
  BoundingBox b2 = other->entityBoundingBoxes;
  float xDiff = 1000;
  float yDiff = 1000;
  float zDiff = 1000;
  float xDir = 1;
  float yDir = 1;
  float zDir = 1;
  switch((b1.boundingX2 - b2.boundingX2)>=0)
  {
    case(0):
      if(b2.boundingX1<b1.boundingX2)
      {
        xDiff = b1.boundingX2 - b2.boundingX1;
        xDir = -1;
      }
      break;
    case(1):
      if(b1.boundingX1<b2.boundingX2)
      {
        xDiff = b2.boundingX2 - b1.boundingX1;

      }
      break;
  }
  switch((b1.boundingY2 - b2.boundingY2)>=0)
  {

    case(0):
      if(b2.boundingY1<b1.boundingY2-.5)
      {
        yDiff = b1.boundingY2-.5 - b2.boundingY1;
        yDir = -1;
      }
      break;
    case(1):
      if(b1.boundingY1-5<b2.boundingY2)
      {
        yDiff = b2.boundingY2 - b1.boundingY1-5;

      }
      break;
  }
  //slog("%f",b1.boundingY2 - b2.boundingY2);
  switch((b1.boundingZ2 - b2.boundingZ2)>=0)
  {
    case(0):
      if(b2.boundingZ1<b1.boundingZ2)
      {
        zDiff = b1.boundingZ2 - b2.boundingZ1;
        zDir = -1;
      }
      break;
    case(1):
      if(b1.boundingZ1<b2.boundingZ2)
      {
        zDiff = b2.boundingZ2 - b1.boundingZ1;
      }
      break;
  }
  //slog("xDiff:%f\tyDiff:%f\tzDiff:%f",xDiff,yDiff,zDiff);
  if(xDiff < yDiff)
  {
    if(xDiff < zDiff)
      return vector3d(xDir,0,0);
    return vector3d(0,0,zDir);
  }
  else if(zDiff<yDiff)
  {
    return vector3d(0,0,zDir);
  }
  else
  {
    return vector3d(0,yDir,0);
  }

}
