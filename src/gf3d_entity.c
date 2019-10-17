#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "gf3d_physics.h"
#include "gf3d_entity.h"

typedef struct
{
    Entity *entity_list;
    Uint32  entity_max;
}EntityManager;

static EntityManager gf3d_entity_manager = {0};

void gf3d_entity_manager_close()
{
    if(gf3d_entity_manager.entity_list != NULL)
    {
        free(gf3d_entity_manager.entity_list);
    }
    memset(&gf3d_entity_manager,0,sizeof(EntityManager));
}

void gf3d_entity_manager_init(Uint32 entity_max)
{
    gf3d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity),entity_max);
    if (!gf3d_entity_manager.entity_list)
    {
        slog("failed to allocate entity list");
        return;
    }
    gf3d_entity_manager.entity_max = entity_max;
    slog("allocated entity list of size %u",sizeof(gf3d_entity_manager.entity_list));
    atexit(gf3d_entity_manager_close);
}

Entity *gf3d_entity_new()
{
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        slog("the _inuse flage for this entity is %u",gf3d_entity_manager.entity_list[i]._inuse);
        if (gf3d_entity_manager.entity_list[i]._inuse)continue;
        //. found a free entity
        memset(&gf3d_entity_manager.entity_list[i],0,sizeof(Entity));
        gf3d_entity_manager.entity_list[i]._inuse = 1;
        return &gf3d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}

void gf3d_entity_free(Entity *self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    gf3d_model_free(self->model);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}

void gf3d_update_all_entities()
{
  int i;
  if(!gf3d_entity_manager.entity_list)
  {
    slog("failed to update entites list is empty");
    return;
  }
  for(i=0;i<gf3d_entity_manager.entity_max;i++)
  {
    Entity *temp = &gf3d_entity_manager.entity_list[i];
    if(temp->_inuse == 0) continue;
    if(temp->isEnvironment) continue;

    update_entity(temp);
    if(temp->isEnvironment2) continue;
    
    if(gf3d_collision_update_entity(temp))
    {
      slog("colliding");
      gf3d_physics_collision_push_back(temp);
    }
  }
}
void update_entity(Entity *e)
{
  gf3d_physics_update(e);

  gf3d_update_entity_bounding_box(e);


}

void gf3d_set_entity_bounding_box(Entity *e)
{

  int i,numBB;
  Mesh * m = e->model->mesh[0];
  if(e->isEnvironment)
  {
    numBB = e->model->mesh[0]->faceCount;
    e->entityBoundingBoxes = (BoundingBox*)gfc_allocate_array(sizeof(BoundingBox),numBB);
    for(i = 0; i< numBB; i++)
    {
      BoundingBox * b = e->entityBoundingBoxes;
      FaceVertex * fv   = m->face_bounding_boxes[i];
      Vector3D max  = find_max_v(fv->vertex,3,1);
      Vector3D min  = find_min_v(fv->vertex,3,1);
      b[i].boundingX1 = min.x;
      b[i].boundingX2 = max.x;
      b[i].boundingY1 = min.y;
      b[i].boundingY2 = max.y;
      b[i].boundingZ1 = min.z;
      b[i].boundingZ2 = max.z;

    }
  }
  else
  {
    e->entityBoundingBoxes = (BoundingBox*)gfc_allocate_array(sizeof(BoundingBox),1);
    BoundingBox * b = e->entityBoundingBoxes;
    e->width = m->max_vertices.x - m->min_vertices.x;
    e->height = m->max_vertices.z - m->min_vertices.z;
    e->depth = m->max_vertices.y - m->min_vertices.y;

    b[0].boundingX1 = e->position.x - e->width/2;
    b[0].boundingX2 = e->position.x + e->width/2;
    b[0].boundingY1 = e->position.y - e->depth/2;
    b[0].boundingY2 = e->position.y + e->depth/2;
    b[0].boundingZ1 = e->position.z - e->height/2;
    b[0].boundingZ2 = e->position.z + e->height/2;
  }
}
void gf3d_update_entity_bounding_box(Entity *e)
{
  int i,numBB;
  Mesh * m = e->model->mesh[0];
  if(e->isEnvironment)
  {

  }
  else
  {
    BoundingBox * b = e->entityBoundingBoxes;
    e->width = m->max_vertices.x - m->min_vertices.x;
    e->height = m->max_vertices.z - m->min_vertices.z;
    e->depth = m->max_vertices.y - m->min_vertices.y;
    //slog("width =%f height =%f depth =%f",width,height,depth);
    b[0].boundingX1 = e->position.x - e->width/2;
    b[0].boundingX2 = e->position.x + e->width/2;
    b[0].boundingY1 = e->position.y - e->depth/2;
    b[0].boundingY2 = e->position.y + e->depth/2;
    b[0].boundingZ1 = e->position.z - e->height/2;
    b[0].boundingZ2 = e->position.z + e->height/2;
  }
}

Entity * gf3d_entity_manager_get_entity(int n)
{
    return &gf3d_entity_manager.entity_list[n];
}

int gf3d_entity_manager_get_size()
{
  return gf3d_entity_manager.entity_max;
}
/*eol@eof*/
