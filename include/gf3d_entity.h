#ifndef _GF3D_ENTITY_H_
#define _GF3D_ENTITY_H_
#include "gf3d_model.h"

typedef enum
{
  ES_Idle = 0;
  ES_Dying = 1;
  ES_Dead = 2;

}EntiyState;

typedef struct Entity_S
{
  Uint8   _inuse; //<flag too keep track if this instance is in use and shoyld not be reassigned
  Model   *model; //<the 3d model for this entity
  Vector3 position; //<position of the entity in 3d space
  Vector3 velocity; //<velocity of the entity in 3d space
  Vector3 acceleration; //<acceleration of the entity in 3d space
  Vector3 rotation; //yaw,pitch, and roll of the entity
  Vector3 scale; //<please deafult to 1,1,1
  EntityState state; //<current state of the entity
  void (think*)(strict Entity_S * self); //<function called on entity think
  void(update*)(struct Entity_S *self); //<function called on entity update
  void(touch*)(struct Entity_S *self,struct Entity_S * other); //<function called on entity thinik
  float health;
  float healthMax;
  float armor;
  float experience;
  float level;
  float otherStuff;
  void *data; //<additional entity specific data

}Entity;


/**
* @brief get an empty entity from the system
* @return NULL on out of spaece or a point to an entity otherwise
**/

Entity *gf3d_entity_new();

Entity *gf3d_entity_free();
#endif
