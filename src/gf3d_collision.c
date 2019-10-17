#include "gf3d_collision.h"
#include "gf3d_entity.h"
#include "gf3d_mesh.h"
#include "simple_logger.h"
Bool gf3d_collision_is_colliding(BoundingBox e,BoundingBox other)
{
  gf3d_collision_print_collision_box(e);
  slog("e^^^");
  gf3d_collision_print_collision_box(other);
  slog("other^^^");
  if(e.boundingX1 <= other.boundingX2 && e.boundingX2 >= other.boundingX1)
  {
    if(e.boundingY1 <= other.boundingY2 && e.boundingY2 >= other.boundingY1)
    {
      return (e.boundingZ1 <= other.boundingZ2 && e.boundingZ2 >= other.boundingZ1);
    }
  }

}

Bool gf3d_collision_update_entity(Entity *e)
{
  int i,j,count;
  Entity * other;
  //slog("collision update");
  if(!e->isEnvironment)
  {
    for(i = 0; i < gf3d_entity_manager_get_size();i++)
    {
      other = gf3d_entity_manager_get_entity(i);
      if(e==other) continue;
      if(other->_inuse == 0) continue;
      if(other->isEnvironment)
      {
        count = other->model->mesh[0]->faceCount;
        for(j = 0; j<count; j++ )
        {
          if(gf3d_collision_is_colliding(e->entityBoundingBoxes[0],other->entityBoundingBoxes[j]))
            return true;

        }

      }
      //slog("%s,%s",e->name,other->name);
      else
      {
        if(gf3d_collision_is_colliding(e->entityBoundingBoxes[0],other->entityBoundingBoxes[0]))
          return true;
      }
    }
  }
  return false;
}

void gf3d_collision_print_collision_box(BoundingBox b)
{
  slog("min coords : [%f,%f,%f]",b.boundingX1,b.boundingY1,b.boundingZ1);
  slog("max coords : [%f,%f,%f]",b.boundingX2,b.boundingY2,b.boundingZ2);
}
