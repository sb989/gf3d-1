#include <SDL.h>
#include <math.h>
#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gf3d_physics.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_create_obj.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *m_wall;
    Matrix4 modelMat;
    Model *m_floor;
    Matrix4 modelMat3;
    Model *testSquare;
    Matrix4 modelMat4;
    Model *mario;
    Matrix4 modelMat5;
    Entity *player;
    Entity *floor;
    Entity *wall;
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }

    init_logger("gf3d.log");
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );

    gf3d_entity_manager_init(100);

    // main game loop
    slog("gf3d main loop begin");

  //  m_wall = gf3d_model_load("wall");
  //  gfc_matrix_identity(modelMat);

    m_floor = gf3d_model_load("floor");
    gfc_matrix_identity(modelMat3);

    testSquare = gf3d_model_load("test_square");
    gfc_matrix_identity(modelMat4);


    mario = gf3d_model_load("mario");
    gfc_matrix_identity(modelMat5);

    gf3d_vgraphics_move_camera(vector3d(0,0,-80));
    gfc_matrix_translate(modelMat5,vector3d(0,0,10));


    player = gf3d_entity_new();
    player->model = mario;
    player->acceleration = vector3d(0,0,-9.8);
    player->velocity = vector3d(0,0,0);
    player->lastUpdate = 0;
    player->isEnvironment = false;
    player->isEnvironment2 = false;
    player->name = "mario";
    gf3d_set_entity_bounding_box(player);

    floor = gf3d_entity_new();
    floor->model = m_floor;
    floor->acceleration = vector3d(0,0,0);
    floor->velocity = vector3d(0,0,0);
    floor->lastUpdate = 0;
    floor->isEnvironment2 = true;
    floor->name = "floor";
    floor->position.x = modelMat3[3][0];
    floor->position.y = modelMat3[3][1];
    floor->position.z = modelMat3[3][2];
    gf3d_set_entity_bounding_box(floor);

  /*  wall = gf3d_entity_new();
    wall->model = m_wall;
    wall->acceleration = vector3d(0,0,0);
    wall->velocity = vector3d(0,0,0);
    wall->lastUpdate = 0;
    wall->isEnvironment2 = true;
    wall->name = "wall";
    wall->position.x = modelMat3[3][0];
    wall->position.y = modelMat3[3][1];
    wall->position.z = modelMat3[3][2];
    gf3d_set_entity_bounding_box(wall);

*/
    player->entityMat = &modelMat5;
    floor->entityMat = &modelMat3;
    //wall->entityMat = &modelMat;
    //player->entityMat = modelMat5;
    gf3d_physics_set_time();
  //  gfc_matrix_translate(modelMat,vector3d(0,-(wall->height)/2,0));
  //  gfc_matrix_translate(modelMat,vector3d(0,0,(wall->height)/2));
    Bool jump = false;
    while(!done)
    {
        count = 0;
        SDL_PumpEvents();   // update SDL's internal event structures

        keys = SDL_GetKeyboardState(NULL);

        if(keys[SDL_SCANCODE_SPACE] && !jump)
        {
          gf3d_physics_set_velocity(player,vector3d(0,0,10));
          jump = true;
        }
        if(!keys[SDL_SCANCODE_SPACE])
        {
          jump = false;
        }
        if(keys[SDL_SCANCODE_A])
        {
          gf3d_physics_set_velocity(player,vector3d(-1,0,0));
        }
        if(keys[SDL_SCANCODE_D])
        {
          gf3d_physics_set_velocity(player,vector3d(1,0,0));
        }
        if(keys[SDL_SCANCODE_W])
        {
          gf3d_physics_set_velocity(player,vector3d(0,1,0));
        }
        if(keys[SDL_SCANCODE_S])
        {
          gf3d_physics_set_velocity(player,vector3d(0,-1,0));
        }
        //g
        // get the keyboard state for this frame
        //update game things here
      //  slog("here3");
        //gf3d_vgraphics_rotate_camera(0.001);

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
              //  gf3d_model_draw(m_wall,bufferFrame,commandBuffer,modelMat,0);
                gf3d_model_draw(m_floor,bufferFrame,commandBuffer,modelMat3,0);
                gf3d_model_draw(testSquare,bufferFrame,commandBuffer,modelMat4,0);
                gf3d_model_draw(mario,bufferFrame,commandBuffer,modelMat5,0);
              //  gfc_matrix_slog(modelMat);
              //  slog("^^^wall");
              slog("-------------------------------------------------------");
                gfc_matrix_slog(modelMat3);
                slog("^^^floor matrix ");
                slog("floor position = [%f,%f,%f] height = %f depth = %f width = %f",floor->position.x,floor->position.y,floor->position.z,floor->height,floor->depth,floor->depth);
                slog("\n");
                gfc_matrix_slog(modelMat5);
                slog("^^^mario");
                slog("mario position = [%f,%f,%f] height = %f depth = %f width = %f",player->position.x,player->position.y,player->position.z,player->height,player->depth,player->depth);
                slog("-------------------------------------------------------");
            gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);
        if(player->lastUpdate == 0){
          player->lastUpdate = gf3d_physics_current_time();
        }
        gf3d_update_all_entities();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
        count++;
        //SDL_PumpEvents();
    }

    //cleanup
    free(floor->entityBoundingBoxes);
    free(player->entityBoundingBoxes);
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
