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
    update_entity(temp);
  }
}
void update_entity(Entity *e)
{
  gf3d_physics_update(e);
  /*gfc_matrix_make_translation(
    e->entityMat,
    e->position
  );*/
  //gfc_matrix_copy(,e->entityMat);
}
/*eol@eof*/
