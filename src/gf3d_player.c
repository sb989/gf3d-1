#include "gf3d_player.h"

typedef struct
{
  Entity * player;
  int walking;// for walking(0 = not walking, 1 = walking )
  int prevDir;// for prevDir(0 = left,  1 = right)
  int dir;    // for dir    (0 = left, 1 = right)
  int falling;// for falling(0 = not falling, 1 = falling)
  int switchDir;
  int walk_left_start_frame;
  int walk_left_end_frame;
  int walk_right_start_frame;
  int walk_right_end_frame;
  int jump_left_start_frame;
  int jump_left_end_frame;
  int jump_right_start_frame;
  int jump_right_end_frame;
  int currentFrame;
  int switch_left_frame;
  int switch_right_frame;
  float lastFrameUpdate;
  int jump;
  int count;
  int isFlat;
  int wasFlat;
  int isTube;
  int wasTube;
  int paper_right_end_frame;
  int paper_left_end_frame;
  int paper_left_start_frame;
  int paper_right_start_frame;
  int tube_left_end_frame;
  int tube_left_start_frame;
  int tube_right_end_frame;
  int tube_right_start_frame;
  int tube_key_pressed;
  float alpha;
}PlayerManager;

static PlayerManager gf3d_player_manager = {0};
Entity * gf3d_player_init()
{
  int k;
  gf3d_player_manager.player = gf3d_entity_init("mario",false,0,61,false);
  slog("first model made");
  gf3d_player_manager.player->secondaryModel = gf3d_model_load_animated("mario_flat",0,53);
  slog("second model made");
  gf3d_player_manager.player->update = &(gf3d_player_update);
  gf3d_player_manager.player->think = &(gf3d_player_think);
  //player->touch = &(gf3d_player_touch);
  gf3d_player_manager.player->acceleration = vector3d(0,0,-9.8);
  slog("mario created");
  gf3d_entity_move(gf3d_player_manager.player,0,0,20);
  gf3d_update_entity_bounding_box(gf3d_player_manager.player,0,0);
  slog("vertex coiunt = %d",gf3d_player_manager.player->model->mesh[0]->vertexCount);
  /*for (k = 0;k<gf3d_player_manager.player->model->mesh[0]->vertexCount;k++)
  {
    Vertex * temp = gf3d_player_manager.player->model->mesh[0]->buffer;
    slog("vertex %d x= %f y= %f x= %f",k,temp[k].vertex.x,temp[k].vertex.y,temp[k].vertex.z);
  }*/
  gf3d_player_manager.walking = 0;
  gf3d_player_manager.prevDir = 0;
  gf3d_player_manager.dir = 0;
  gf3d_player_manager.falling = 0;
  gf3d_player_manager.walk_left_start_frame = 0;
  gf3d_player_manager.walk_left_end_frame = 15;//16;
  gf3d_player_manager.jump_left_start_frame = 16;
  gf3d_player_manager.jump_left_end_frame = 20;
  gf3d_player_manager.walk_right_start_frame = 31;
  gf3d_player_manager.walk_right_end_frame = 46;//47;
  gf3d_player_manager.jump_right_start_frame = 47;
  gf3d_player_manager.jump_right_end_frame = 51;
  gf3d_player_manager.switch_left_frame = 24;//23;
  gf3d_player_manager.switch_right_frame = 31;
  gf3d_player_manager.currentFrame = 0;
  gf3d_player_manager.switchDir = 0;
  gf3d_player_manager.lastFrameUpdate = 0;
  gf3d_player_manager.jump = 0;
  gf3d_player_manager.count = 0;
  gf3d_player_manager.isFlat = 0;
  gf3d_player_manager.isTube = 0;
  gf3d_player_manager.wasFlat = 0;
  gf3d_player_manager.wasTube = 0;
  gf3d_player_manager.paper_left_start_frame = 0;
  gf3d_player_manager.paper_left_end_frame = 13;
  gf3d_player_manager.paper_right_start_frame = 15;
  gf3d_player_manager.paper_right_end_frame = 28;
  gf3d_player_manager.tube_left_end_frame = 40;
  gf3d_player_manager.tube_left_start_frame = 29;
  gf3d_player_manager.tube_right_end_frame = 52;
  gf3d_player_manager.tube_right_start_frame = 41;
  gf3d_player_manager.tube_key_pressed = 0;
  gf3d_player_manager.player->rotation = vector3d(0,0,0);
  gf3d_player_manager.alpha = 0;
  return gf3d_player_manager.player;
}

void gf3d_player_think()
{
  Entity * e = gf3d_player_manager.player;
  //slog("thinking");
  const Uint8 * keys;
  keys = SDL_GetKeyboardState(NULL);
  if(keys[SDL_SCANCODE_SPACE] && !gf3d_player_manager.jump && e->velocity.z == 0)
  {
    e->velocity.z = 15;
    gf3d_player_manager.falling = 1;
    gf3d_player_manager.jump = 1;
  }
  if(!keys[SDL_SCANCODE_SPACE])
  {
    gf3d_player_manager.jump = 0;
  }
  if(keys[SDL_SCANCODE_A])
  {
    e->velocity.x = 2;
  //  slog("pressing a");
  //  slog("before dir is changed dir is %d",gf3d_player_manager.dir);
    gf3d_player_manager.prevDir = gf3d_player_manager.dir;
    gf3d_player_manager.dir = 0;
  //  slog("after dir is changed dir is %d",gf3d_player_manager.dir);
  //  slog("after dir is changed prevDir i %d",gf3d_player_manager.prevDir);
    if(gf3d_player_manager.prevDir != gf3d_player_manager.dir)
    {
      gf3d_player_manager.switchDir = 1;
    }
    gf3d_player_manager.walking = 1;
    if(gf3d_player_manager.isTube)
    {
      float angle;
      angle = .01;
      gfc_matrix_rotate(gf3d_player_manager.player->entityMat,gf3d_player_manager.player->entityMat,angle,vector3d(0,0,1));

      e->rotation.z = e->rotation.z + angle;

      gf3d_rotate_entity_bounding_box(e,1,gf3d_player_manager.currentFrame,e->rotation.z);
      e->velocity.x = 0;
    }
  }
  else if(keys[SDL_SCANCODE_D])
  {
    e->velocity.x = -2;
  //  slog("pressing d");
  //  slog("before dir is changed dir is %d",gf3d_player_manager.dir);
    gf3d_player_manager.prevDir = gf3d_player_manager.dir;
    gf3d_player_manager.dir = 1;
    //slog("after dir is changed dir is %d",gf3d_player_manager.dir);
    //slog("after dir is changed prevDir i %d",gf3d_player_manager.prevDir);
    if(gf3d_player_manager.prevDir != gf3d_player_manager.dir)
    {
      gf3d_player_manager.switchDir = 1;
    }
    gf3d_player_manager.walking = 1;
    if(gf3d_player_manager.isTube)
    {
      float angle;
      angle = -.01;
      gfc_matrix_rotate(gf3d_player_manager.player->entityMat,gf3d_player_manager.player->entityMat,angle,vector3d(0,0,1));

      e->rotation.z = e->rotation.z + angle;

      gf3d_rotate_entity_bounding_box(e,1,gf3d_player_manager.currentFrame,e->rotation.z);
      e->velocity.x = 0;
    }
  }
  else
  {
    e->velocity.x = 0;
    //gf3d_player_manager.walking = 0;
  }
  if(keys[SDL_SCANCODE_W])
  {

    //r = gf3d_player_manager.player->depth;
    e->velocity.y = -2;
    gf3d_player_manager.walking = 1;
    if(gf3d_player_manager.isTube)
    {
      Vector3D temp;
      temp = gf3d_player_manager.player->position;
      temp.x = temp.x+1;
      gf3d_entity_rotate_point(&gf3d_player_manager.player->position,&temp,gf3d_player_manager.player->rotation.z+gf3d_player_manager.alpha);

      e->velocity.y = temp.y - gf3d_player_manager.player->position.y;
      e->velocity.x = temp.x - gf3d_player_manager.player->position.x;
    }
  }
  else if(keys[SDL_SCANCODE_S])
  {
    e->velocity.y = 2;
    gf3d_player_manager.walking = 1;
    if(gf3d_player_manager.isTube)
    {
      Vector3D temp;
      temp = gf3d_player_manager.player->position;
      temp.x = temp.x-1;
      gf3d_entity_rotate_point(&gf3d_player_manager.player->position,&temp,gf3d_player_manager.player->rotation.z+gf3d_player_manager.alpha);

      e->velocity.y = temp.y - gf3d_player_manager.player->position.y;
      e->velocity.x = temp.x - gf3d_player_manager.player->position.x;
    }
  }
  else
  {
    e->velocity.y = 0;
    if(gf3d_player_manager.isTube)
      e->velocity.x = 0;
    //gf3d_player_manager.walking = 0;
  }
  if(!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_W])
    {
      gf3d_player_manager.walking = 0;
    }
  if(keys[SDL_SCANCODE_J])
  {
    if(!gf3d_player_manager.isTube)
      gf3d_player_manager.isFlat = 1;
    else
    {
      gf3d_player_manager.isFlat = 0;
    }
  }
  else
  {
    gf3d_player_manager.isFlat = 0;
  }
  if(keys[SDL_SCANCODE_I])
  {

    if(gf3d_player_manager.isFlat && !gf3d_player_manager.isTube)
    {
      gf3d_player_manager.isTube = 1;
      gf3d_player_manager.tube_key_pressed = 1;
      //gf3d_physics_teleport(e,vector3d(0,0,10));
      e->velocity.z = 15;
    }
    else if(!gf3d_player_manager.isFlat && gf3d_player_manager.isTube && gf3d_player_manager.wasTube)
    {
      //e->velocity.z = 10;
      if(!gf3d_player_manager.tube_key_pressed)
      {
        gf3d_player_manager.tube_key_pressed = 1;
        gf3d_player_manager.wasTube = gf3d_player_manager.isTube;
        gf3d_player_manager.isTube = 0;
        e->velocity.z = 10;
        gf3d_physics_teleport(e,vector3d(0,0,10));
        slog("out of tube mode");
      }
      //gf3d_physics_teleport(e,vector3d(0,0,10));
      //e->velocity.z = 15;
    }
  }
  else
  {
    gf3d_player_manager.tube_key_pressed = 0;
  }
  //slog("calling update");
  gf3d_player_update(e);
}

void gf3d_player_update(Entity * e)
{
  //gf3d_player_manager.count = gf3d_player_manager.count +1;
  //slog("%d",gf3d_player_manager.count);
  //slog("update");
  if(!gf3d_player_manager.isFlat)
  {
    update_entity(e,0,0);
  }
  else if(gf3d_player_manager.isFlat && !gf3d_player_manager.wasTube )
  {
    update_entity(e,1,gf3d_player_manager.currentFrame);
  }
  if(gf3d_player_manager.isTube)
  {

    gf3d_physics_update(e);
    gf3d_rotate_entity_bounding_box(e,1,gf3d_player_manager.currentFrame,e->rotation.z);
    slog("in here");
    gf3d_player_manager.player->position.z = gf3d_player_manager.player->position.z +5;
    e->entityBoundingBoxes.boundingZ1 = e->entityBoundingBoxes.boundingZ1 + 5;
    e->entityBoundingBoxes.boundingZ2 = e->entityBoundingBoxes.boundingZ2-5;
  }
  gf3d_collision_update_entity(e);
  //slog("z velocity %f",e->velocity.z);
  // if velocity in z direction is anything other than zero set falling to true
  if(e->velocity.z>= -.1 && e->velocity.z<=.1)
  {
    gf3d_player_manager.falling = 0;
  }

  //gf3d_player_update_frame();
 //slog("%f",gf3d_player_manager.lastFrameUpdate);
 //slog("%f",gf3d_physics_current_time()/(CLOCKS_PER_SEC));
 if(gf3d_player_manager.lastFrameUpdate == 0 || ((gf3d_physics_current_time()/CLOCKS_PER_SEC-gf3d_player_manager.lastFrameUpdate) > .02))
  {
    gf3d_player_update_frame();
    gf3d_player_manager.lastFrameUpdate = (gf3d_physics_current_time()/CLOCKS_PER_SEC);
  }
  Matrix4 mat;
  gfc_matrix_copy(mat,*(*(gf3d_player_manager.player->entityMat)));
  if(gf3d_player_manager.isTube)
  {
    //for(int x = 0; x <4; x++)
      //slog("[%f,%f,%f,%f]\n",mat[x][0],mat[x][1],mat[x][2],mat[x][3]);
  }
  //slog("-----------------------");

}
void gf3d_player_update_frame()
{
  //if player is falling set walking to false (same animation is used for falling and jumping)
  if(gf3d_player_manager.falling && !gf3d_player_manager.wasFlat &&!gf3d_player_manager.wasTube)
  {
    slog("mario is falling");
    //gf3d_player_manager.walking = 0;
    //change direction of falling animation depending on direction
    if(gf3d_player_manager.dir)
    {
      gf3d_player_manager.currentFrame = 49;
      return;
    }
    else
    {
      gf3d_player_manager.currentFrame = 18;
      return;
    }
  }
  //sets first frame of animation for switching to paper (makes sure you arent changing direction at the time)
  if(gf3d_player_manager.isFlat && !gf3d_player_manager.wasFlat && !gf3d_player_manager.switchDir)
  {
    gf3d_player_manager.wasFlat = 1;
    if(gf3d_player_manager.dir)
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.paper_right_start_frame;
      return;
    }
    else
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.paper_left_start_frame;
      return;
    }
  }
  //runs through the frames of the switch animation
  if(gf3d_player_manager.isFlat && gf3d_player_manager.wasFlat && !gf3d_player_manager.switchDir)
  {
    if(!gf3d_player_manager.dir)
    {
      if((gf3d_player_manager.currentFrame < gf3d_player_manager.paper_left_end_frame))
      {
        gf3d_player_manager.currentFrame  = gf3d_player_manager.currentFrame + 1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.paper_left_end_frame;
        return;
      }
    }
    else
    {
      if((gf3d_player_manager.currentFrame < gf3d_player_manager.paper_right_end_frame))
      {
        gf3d_player_manager.currentFrame  = gf3d_player_manager.currentFrame + 1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.paper_right_end_frame;
        return;
      }
    }
  }
  //checks if in paper mode before turning into tube
  if(gf3d_player_manager.isTube && !gf3d_player_manager.wasTube)
  {
    gf3d_player_manager.isFlat = 0;
    gf3d_player_manager.wasTube = 1;
    if(gf3d_player_manager.dir)
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.tube_right_start_frame;
      gf3d_player_manager.alpha= 10*M_PI/180;
      return;
    }
    else
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.tube_left_start_frame;
      gf3d_player_manager.alpha = -10*M_PI/180;
      return;
    }
  }
  if(gf3d_player_manager.isTube && gf3d_player_manager.wasTube)
  {
    if(gf3d_player_manager.isTube < 2 )
    {
      if(gf3d_player_manager.dir)
      {
        if(gf3d_player_manager.currentFrame < gf3d_player_manager.tube_right_end_frame)
        {
          gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
          return;
        }
        else
        {
          gf3d_player_manager.currentFrame = gf3d_player_manager.tube_right_end_frame;
          gf3d_player_manager.isTube = 3;
          return;
        }
      }
      else
      {
        if(gf3d_player_manager.currentFrame < gf3d_player_manager.tube_left_end_frame)
        {
          gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
          return;
        }
        else
        {
          gf3d_player_manager.currentFrame = gf3d_player_manager.tube_left_end_frame;
          gf3d_player_manager.isTube = 4;
          return;
        }
      }
    }
    else
      return;

  }

  if(!gf3d_player_manager.isTube && gf3d_player_manager.wasTube)
  {
    if(gf3d_player_manager.wasTube == 3)
    {
      if(gf3d_player_manager.currentFrame > gf3d_player_manager.tube_right_start_frame)
      {
          slog("minus on line 435");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame -1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.paper_right_end_frame;
        gf3d_player_manager.wasTube = 0;
        gfc_matrix_rotate(gf3d_player_manager.player->entityMat,gf3d_player_manager.player->entityMat,-gf3d_player_manager.player->rotation.z,vector3d(0,0,1));
        gf3d_player_manager.player->rotation.z = 0;
        gf3d_player_manager.dir = 1;
        gf3d_player_manager.switchDir = 0;
        gf3d_player_manager.isFlat = 0;
        gf3d_player_manager.wasFlat = 1;
        return;
      }
    }
    if(gf3d_player_manager.wasTube ==4)
    {
      if(gf3d_player_manager.currentFrame > gf3d_player_manager.tube_left_start_frame)
      {
        slog("minus on line 452");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame -1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.paper_left_end_frame;
        gf3d_player_manager.wasTube = 0;
        gf3d_player_manager.dir = 0;
        gfc_matrix_rotate(gf3d_player_manager.player->entityMat,gf3d_player_manager.player->entityMat,-gf3d_player_manager.player->rotation.z,vector3d(0,0,1));
        gf3d_player_manager.player->rotation.z = 0;
        gf3d_player_manager.switchDir = 0;
        gf3d_player_manager.isFlat = 0;
        gf3d_player_manager.wasFlat = 1;
        return;
      }
    }
  }
  //chages direction of model in paper mode
  if(gf3d_player_manager.isFlat && gf3d_player_manager.wasFlat && gf3d_player_manager.switchDir)
  {
    if(gf3d_player_manager.currentFrame == gf3d_player_manager.paper_right_end_frame)
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.paper_left_end_frame;
      gf3d_player_manager.switchDir = 0;
      return;
    }
    if(gf3d_player_manager.currentFrame == gf3d_player_manager.paper_left_end_frame)
    {
      gf3d_player_manager.switchDir = 0;
      gf3d_player_manager.currentFrame = gf3d_player_manager.paper_right_end_frame;
      return;
    }

  }



  //runs the animation in reverse to return to normal state from paper state
  if(!gf3d_player_manager.isFlat && gf3d_player_manager.wasFlat)
  {
    if(!gf3d_player_manager.dir)
    {
      if(gf3d_player_manager.currentFrame > gf3d_player_manager.paper_left_start_frame)
      {
          slog("minus on line 493");
        gf3d_player_manager.currentFrame  = gf3d_player_manager.currentFrame - 1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_left_start_frame;
        gf3d_player_manager.wasFlat = 0;
        return;
      }
    }
    else
    {
      if(gf3d_player_manager.currentFrame >gf3d_player_manager.paper_right_start_frame)
      {
          slog("minus on line 508");
        gf3d_player_manager.currentFrame  = gf3d_player_manager.currentFrame - 1;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_right_start_frame;
        gf3d_player_manager.wasFlat = 0;
        return;
      }
    }
  }

  //handles all cases where player is walking and is not paper or tube
  if(gf3d_player_manager.walking)
  {
    //slog("walking");
    //if player changes direction change animation
    //slog("dir = %d prevDir = %d siwtchDir = %d",gf3d_player_manager.dir,gf3d_player_manager.prevDir,gf3d_player_manager.switchDir);
    if(gf3d_player_manager.switchDir == 1)
    {
      slog("switch direction");
      if(gf3d_player_manager.dir == 1)
      {
        //slog("current frame is switch left frame ");
        gf3d_player_manager.currentFrame = gf3d_player_manager.switch_left_frame;
        gf3d_player_manager.switchDir = 2;
        return;
      }
      else
      {
        //slog("current frame is switch right frame");
        gf3d_player_manager.currentFrame = gf3d_player_manager.switch_right_frame;
        gf3d_player_manager.switchDir = 2;
        return;
      }


    }
    //slog("switchDir2222 = %d",gf3d_player_manager.switchDir);
    //determines if player is finished changing direction from left to right
    if(gf3d_player_manager.dir == 1 && gf3d_player_manager.switchDir)
    {
      //slog("dir == 1 and switchDir == 1");
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.switch_right_frame)
      {
        //slog("done transitioning from left to right");
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_right_start_frame;
        gf3d_player_manager.switchDir = 0;
        //gf3d_player_manager.prevDir = gf3d_player_manager.dir;
        return;
      }
      else
      {
        //slog("still transitioning from left to right");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
        return;
      }
    }
    //determines if player is finisehd changing direction from right to left
    if(gf3d_player_manager.dir ==0 && gf3d_player_manager.switchDir)
    {
      //slog("dir == 0 and switchDir == 1");
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.switch_left_frame)
      {
        //slog("done transitioning from right to left");
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_left_start_frame;
        gf3d_player_manager.switchDir = 0;
        //gf3d_player_manager.prevDir = gf3d_player_manager.dir;
        return;
      }
      else
      {
        //slog("still transitioning from right to left");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame -1;
        return;
      }
    }
    //updates player frame when walking right
    if(gf3d_player_manager.dir && !gf3d_player_manager.switchDir)
    {
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.walk_right_end_frame || gf3d_player_manager.currentFrame > gf3d_player_manager.walk_right_end_frame || gf3d_player_manager.currentFrame < gf3d_player_manager.walk_right_start_frame)
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_right_start_frame;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
        return;
      }
    }
    //updates player frame when walking left
    if(!gf3d_player_manager.dir && !gf3d_player_manager.switchDir)
    {
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.walk_left_end_frame|| gf3d_player_manager.currentFrame > gf3d_player_manager.walk_left_end_frame || gf3d_player_manager.currentFrame < gf3d_player_manager.walk_left_start_frame)
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_left_start_frame;
        return;
      }
      else
      {
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
        return;
      }
    }

  }
  //loads
  if(!gf3d_player_manager.walking)
  {
    //slog("not walking");
    //slog("dir = %d prevDir = %d siwtchDir = %d",gf3d_player_manager.dir,gf3d_player_manager.prevDir,gf3d_player_manager.switchDir);

    //slog("switchDir2222 = %d",gf3d_player_manager.switchDir);
    //determines if player is finished changing direction from left to right
    if(gf3d_player_manager.dir == 1 && gf3d_player_manager.switchDir)
    {
      //slog("wring one 1");
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.switch_right_frame)
      {
        //slog("wrong one 1 end");
        gf3d_player_manager.currentFrame =gf3d_player_manager.walk_right_start_frame;
        gf3d_player_manager.switchDir = 0;
        //gf3d_player_manager.prevDir = gf3d_player_manager.dir;
        return;
      }
      else
      {
        //slog("wrong one 1 transition");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame +1;
        return;
      }
    }
    //determines if player is finisehd changing direction from right to left
    if(gf3d_player_manager.dir ==0 && gf3d_player_manager.switchDir)
    {
      //slog("wrong one 2");
      if(gf3d_player_manager.currentFrame == gf3d_player_manager.switch_left_frame)
      {
        //slog("wrong one 2 end");
        gf3d_player_manager.currentFrame = gf3d_player_manager.walk_left_start_frame;
        gf3d_player_manager.switchDir = 0;
        //gf3d_player_manager.prevDir = gf3d_player_manager.dir;
        return;
      }
      else
      {
        //slog("wrong one 2 transition");
        gf3d_player_manager.currentFrame = gf3d_player_manager.currentFrame -1;
        return;
      }
    }
    if(gf3d_player_manager.dir)
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.walk_right_start_frame;
      return;
    }
    if(!gf3d_player_manager.dir)
    {
      gf3d_player_manager.currentFrame = gf3d_player_manager.walk_left_start_frame;
      return;
    }
  }
}
void gf3d_player_draw(Uint32 bufferFrame,VkCommandBuffer commandBuffer)
{
  if(!gf3d_player_manager.wasFlat)
  {
    gf3d_model_draw(gf3d_player_manager.player->model,bufferFrame,commandBuffer,(*(gf3d_player_manager.player)->entityMat),gf3d_player_manager.currentFrame);
  }
  else
  {
    gf3d_model_draw(gf3d_player_manager.player->secondaryModel,bufferFrame,commandBuffer,(*(gf3d_player_manager.player)->entityMat),gf3d_player_manager.currentFrame);
  }
  //gf3d_entity_draw(gf3d_player_manager.player,gf3d_player_manager.currentFrame,bufferFrame,commandBuffer);
}
