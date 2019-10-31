#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf3d_model.h"

typedef enum
{
    ES_Idle = 0,
    ES_Dying = 1,
    ES_Dead = 2
}EntityState;

typedef struct
{
  float           boundingX1;
  float           boundingX2;
  float           boundingY1;
  float           boundingY2;
  float           boundingZ1;
  float           boundingZ2;
}BoundingBox;

typedef struct
{
  Vector3D planeVert[4];
}Plane;

typedef struct
{
  Plane xy1;
  Plane xy2;
  Plane xz1;
  Plane xz2;
  Plane yz1;
  Plane yz2;
}CubePlane;

typedef struct Entity_S
{
    Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Model          *model;          /**<the 3d model for this entity*/
    Model          *secondaryModel;  /**<secondary 3d model for entity if needed*/
    Vector3D        position;       /**<position of the entity in 3d space*/
    Vector3D        velocity;       /**<velocity of the entity in 3d space*/
    Vector3D        acceleration;   /**<acceleration of the entity in 3d space*/
    Vector3D        rotation;       /**<yaw, pitch, and roll of the entity*/
    Vector3D        scale;          /**<*please default to 1,1,1*/
    EntityState     state;          /**<current state of the entity*/
    void (*think) (struct Entity_S* self);   /**<function called on entity think*/
    void (*update)(struct Entity_S* self);   /**<function called on entity update*/
    void (*touch) (struct Entity_S* self,struct Entity_S* other);   /**<function called on entity think*/
    float           health;
    float           healthmax;
    float           armor;
    float           experience;
    float           level;
    double           lastUpdate;
    float           otherStuff;
    BoundingBox     entityBoundingBoxes;
    Matrix4         *entityMat;
    Bool            isEnvironment;
    Bool            isEnvironment2;
    float           width;
    float           depth;
    float           height;
    char            *name;
    int             mass;
    Vector3D        lastVel;
    CubePlane       cp;
    int             numAnimations;
    Bool            isEnemy;
    void *data;                     /**<additional entity specific data*/

}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void    gf3d_entity_free(Entity *self);
void gf3d_update_all_entities();
void update_entity(Entity *e,int num,int frame);
void gf3d_entity_sync_position(Entity *e);
void gf3d_set_entity_bounding_box(Entity *e);
Entity * gf3d_entity_init(char * model, Bool isEnvironment,int startFrame,int endFrame,Bool isEnemy);
void gf3d_entity_move(Entity * e,float x,float y,float z);
void gf3d_entity_draw(Entity * e,int frame,Uint32 bufferFrame,VkCommandBuffer commandBuffer);
int gf3d_entity_manager_get_size();
Entity * gf3d_entity_manager_get_entity(int n);
void gf3d_rotate_entity_bounding_box(Entity *e,int num,int frame,float angle);
void gf3d_entity_rotate_point(Vector3D * pivot, Vector3D * rotate,float angle);

void gf3d_entity_setup_cube_plane(Entity * e);
#endif
