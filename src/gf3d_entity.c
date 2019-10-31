#include <stdlib.h>
#include <string.h>
#include "gfc_list.h"
#include "simple_logger.h"
#include "gf3d_physics.h"
#include "gf3d_entity.h"
#include "gf3d_model.h"
#include "gf3d_collision.h"

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
    free(self->entityMat);
    gf3d_model_free(self->model);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}

Entity * gf3d_entity_init(char * model, Bool isEnvironment,int startFrame,int endFrame, Bool isEnemy)
{
  Entity * temp = gf3d_entity_new();
  Model * m;
  int numAnimations = endFrame-startFrame;
  if(numAnimations <= 0)
  {
    //gf3d_model_free(model);
    free(temp);
    slog("frameRange is zero or negative: %i",numAnimations);
    return NULL;
  }
  else if(numAnimations ==1)
  {
    m = gf3d_model_load(model);
  }
  else
  {
    m = gf3d_model_load_animated(model,startFrame,endFrame);
  }
  slog("loaded model successfully");
  Matrix4 * mat;
  mat = (Matrix4 *)malloc(sizeof(Matrix4));
  gfc_matrix_identity(*mat);
  temp->model = m;
  temp->entityMat = mat;
  temp->isEnvironment2 = isEnvironment;
  temp->name = model;
  temp->lastUpdate = 0;
  temp->velocity = vector3d(0,0,0);
  temp->acceleration = vector3d(0,0,0);
  temp->lastUpdate = 0;
  temp->isEnemy = isEnemy;
  slog("before");
  gf3d_set_entity_bounding_box(temp);
  slog("created bounding box successfully");
  temp->numAnimations = numAnimations;
  return temp;
}

void gf3d_entity_move(Entity * e,float x,float y,float z)
{
  gfc_matrix_translate(e->entityMat,vector3d(x,0,0));
  gfc_matrix_translate(e->entityMat,vector3d(0,y,0));
  gfc_matrix_translate(e->entityMat,vector3d(0,0,z));
}

void gf3d_entity_draw(Entity * e,int frame,Uint32 bufferFrame,VkCommandBuffer commandBuffer)
{
  if(e->state != ES_Dead)
  gf3d_model_draw(e->model,bufferFrame,commandBuffer,(*e->entityMat),frame);
}


void gf3d_update_all_entities()
{
  int i, j;
  //List * collisions;

  if(!gf3d_entity_manager.entity_list)
  {
    slog("failed to update entites list is empty");
    return;
  }
  for(i=0;i<gf3d_entity_manager.entity_max;i++)
  {
    Entity *temp = &gf3d_entity_manager.entity_list[i];
    if(temp->_inuse == 0) continue;
    update_entity(temp,0,0);
    if(temp->isEnvironment2) continue;

    //collisions = gfc_list_new();
    gf3d_collision_update_entity(temp);


  }
}
void update_entity(Entity *e, int num,int frame)
{
//  slog("update");
  gf3d_physics_update(e);

  gf3d_update_entity_bounding_box(e,num,frame);
}

void gf3d_set_entity_bounding_box(Entity *e)
{

//  int i,numBB;
    Mesh * m = e->model->mesh[0];

    //slog("befiore2");
    //e->entityBoundingBoxes = (BoundingBox*)gfc_allocate_array(sizeof(BoundingBox),1);
    //BoundingBox b = e->entityBoundingBoxes;
    gf3d_entity_sync_position(e);
    //slog("positions synced");
    e->width = m->max_vertices.x - m->min_vertices.x;
    e->height = m->max_vertices.z - m->min_vertices.z;
    e->depth = m->max_vertices.y - m->min_vertices.y;
    slog("width =%f height =%f depth =%f",e->width,e->height,e->depth);
    e->entityBoundingBoxes.boundingX1 = e->position.x - e->width/2;
    e->entityBoundingBoxes.boundingX2 = e->position.x + e->width/2;
    e->entityBoundingBoxes.boundingY1 = e->position.y - e->depth/2;
    e->entityBoundingBoxes.boundingY2 = e->position.y + e->depth/2;
    e->entityBoundingBoxes.boundingZ1 = e->position.z - e->height/2;
    e->entityBoundingBoxes.boundingZ2 = e->position.z + e->height/2;
    //gf3d_entity_setup_cube_plane(e);
    //BoundingBox b = e->entityBoundingBoxes;
    //gf3d_collision_print_collision_box(b);
}
void gf3d_update_entity_bounding_box(Entity *e,int num,int frame)
{
  int i,numBB;
  Mesh * m;
  if(!num)
    m = e->model->mesh[frame];
  else
    m = e->secondaryModel->mesh[frame];
  gf3d_entity_sync_position(e);

  e->width = m->max_vertices.x - m->min_vertices.x;
  e->height = m->max_vertices.z - m->min_vertices.z;
  e->depth = m->max_vertices.y - m->min_vertices.y;
  //slog("width =%f height =%f depth =%f",e->width,e->height,e->depth);
  e->entityBoundingBoxes.boundingX1 = e->position.x - e->width/2;
  e->entityBoundingBoxes.boundingX2 = e->position.x + e->width/2;
  e->entityBoundingBoxes.boundingY1 = e->position.y - e->depth/2;
  e->entityBoundingBoxes.boundingY2 = e->position.y + e->depth/2;
  e->entityBoundingBoxes.boundingZ1 = e->position.z - e->height/2;
  e->entityBoundingBoxes.boundingZ2 = e->position.z + e->height/2;

}
//rotates box along z axis
void gf3d_rotate_entity_bounding_box(Entity *e,int num,int frame,float angle)
{
  Mesh * m;
  float x1,x2,y1,y2,z1,z2;
  int d;
  if(!num)
    m = e->model->mesh[frame];
  else
    m = e->secondaryModel->mesh[frame];
  x1 = e->entityBoundingBoxes.boundingX1;
  y1 = e->entityBoundingBoxes.boundingY1;
  z1 = e->entityBoundingBoxes.boundingZ1;
  x2 = e->entityBoundingBoxes.boundingX2;
  y2 = e->entityBoundingBoxes.boundingY2;
  z2 = e->entityBoundingBoxes.boundingZ2;
  Vector3D points [24] = {vector3d(x1,y1,z1),vector3d(x2,y1,z1),
  vector3d(x2,y2,z1),vector3d(x1,y2,z1),vector3d(x1,y1,z2),
  vector3d(x2,y1,z2),vector3d(x2,y2,z2),vector3d(x1,y2,z2),
  vector3d(x1,y1,z1),vector3d(x2,y1,z1),vector3d(x2,y1,z2),
  vector3d(x1,y1,z2),vector3d(x1,y2,z1),vector3d(x2,y2,z1),
  vector3d(x2,y2,z2),vector3d(x1,y2,z2),vector3d(x1,y1,z1),
  vector3d(x1,y1,z2),vector3d(x1,y2,z2),vector3d(x1,y2,z1),
  vector3d(x2,y1,z1),vector3d(x2,y1,z2),vector3d(x2,y2,z2),
  vector3d(x2,y2,z1)};
  slog("-------------------");
  for(d=0;d<24;d++)
  {

    gf3d_entity_rotate_point(&e->position,&points[d],angle);
  }
  slog("-------------------");
  m->max_vertices = find_max_v(&points,24,1);
  m->min_vertices = find_min_v(&points,24,1);
  float temp = m->max_vertices.x;
  m->max_vertices.x = m->max_vertices.y;
  m->max_vertices.y = temp;
  temp = m->min_vertices.x;
  m->min_vertices.x = m->min_vertices.y;
  m->min_vertices.y = temp;
  //slog("max vrerts = [%f,%f,%f]",m->max_vertices.x,m->max_vertices.y,m->max_vertices.z);
  gf3d_update_entity_bounding_box(e,num,frame);
  slog("width =%f height =%f depth =%f",e->width,e->height,e->depth);
}

void gf3d_entity_rotate_point(Vector3D * pivot, Vector3D * rotate,float angle)
{
  float x,y;
  x = cos(angle) * (rotate->x -pivot->x) - sin(angle) *(rotate->y-pivot->y) + pivot->x;
  y = sin(angle) * (rotate->x -pivot->x) + cos(angle) *(rotate->y-pivot->y) + pivot->y;
  //float r = sqrt((rotate->x-pivot->x)*(rotate->x-pivot->x) + (rotate->y-pivot->y)*(rotate->y-pivot->y));
  //slog("r = %f",r);
  //slog("angle = %f",angle);
  //x = 1/r;
  //x = x/(sqrt(2-2*cos(angle)));
  //y = x;
  //x = x*(sin(-angle/2));
  //y = y*(cos(-angle/2));
  slog("%f,%f",x,y);
  rotate->x = x;//rotate->x +x;
  rotate->y = y;//rotate->y+y;
}
Entity * gf3d_entity_manager_get_entity(int n)
{
    return &gf3d_entity_manager.entity_list[n];
}
void gf3d_entity_sync_position(Entity *e)
{
  e->position.x = (*e->entityMat)[3][0];
  e->position.y = (*e->entityMat)[3][1];
  e->position.z = (*e->entityMat)[3][2];
}
int gf3d_entity_manager_get_size()
{
  return gf3d_entity_manager.entity_max;
}

void gf3d_entity_setup_cube_plane(Entity * e)
{

  Mesh * m = e->model->mesh[0];
  float x1,x2,y1,y2,z1,z2;
  x1 = e->entityBoundingBoxes.boundingX1;
  y1 = e->entityBoundingBoxes.boundingY1;
  z1 = e->entityBoundingBoxes.boundingZ1;
  x2 = e->entityBoundingBoxes.boundingX2;
  y2 = e->entityBoundingBoxes.boundingY2;
  z2 = e->entityBoundingBoxes.boundingZ2;

  e->cp.xy1.planeVert[0] = vector3d(x1,y1,z1);
  e->cp.xy1.planeVert[1] = vector3d(x2,y1,z1);
  e->cp.xy1.planeVert[2] = vector3d(x2,y2,z1);
  e->cp.xy1.planeVert[3] = vector3d(x1,y2,z1);

  e->cp.xy2.planeVert[0] = vector3d(x1,y1,z2);
  e->cp.xy2.planeVert[1] = vector3d(x2,y1,z2);
  e->cp.xy2.planeVert[2] = vector3d(x2,y2,z2);
  e->cp.xy2.planeVert[3] = vector3d(x1,y2,z2);

  e->cp.xz1.planeVert[0] = vector3d(x1,y1,z1);
  e->cp.xz1.planeVert[1] = vector3d(x2,y1,z1);
  e->cp.xz1.planeVert[2] = vector3d(x2,y1,z2);
  e->cp.xz1.planeVert[3] = vector3d(x1,y1,z2);

  e->cp.xz2.planeVert[0] = vector3d(x1,y2,z1);
  e->cp.xz2.planeVert[1] = vector3d(x2,y2,z1);
  e->cp.xz2.planeVert[2] = vector3d(x2,y2,z2);
  e->cp.xz2.planeVert[3] = vector3d(x1,y2,z2);

  e->cp.yz1.planeVert[0] = vector3d(x1,y1,z1);
  e->cp.yz1.planeVert[1] = vector3d(x1,y1,z2);
  e->cp.yz1.planeVert[2] = vector3d(x1,y2,z2);
  e->cp.yz1.planeVert[3] = vector3d(x1,y2,z1);

  e->cp.yz2.planeVert[0] = vector3d(x2,y1,z1);
  e->cp.yz2.planeVert[1] = vector3d(x2,y1,z2);
  e->cp.yz2.planeVert[2] = vector3d(x2,y2,z2);
  e->cp.yz2.planeVert[3] = vector3d(x2,y2,z1);

}

/*eol@eof*/
