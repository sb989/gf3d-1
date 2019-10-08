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

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model;
    Matrix4 modelMat;
    Model *model2;
    Matrix4 modelMat2;
    Model *model3;
    Matrix4 modelMat3;
    Model *testSquare;
    Matrix4 modelMat4;
    Model *mario;
    Matrix4 modelMat5;
    Entity *player;
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

    gf3d_entity_manager_init(10);

    // main game loop
    slog("gf3d main loop begin");


    model3 = gf3d_model_load("basic_level_3");
    //slog("house");
    gfc_matrix_identity(modelMat3);

    testSquare = gf3d_model_load("test_square");
    gfc_matrix_identity(modelMat4);


    mario = gf3d_model_load("mario");
    gfc_matrix_identity(modelMat5);

    //slog("finish dino 2");


    gfc_matrix_make_translation(
            modelMat3,
            vector3d(0,0,-10)
        );
    gfc_matrix_make_translation(
            modelMat4,
            vector3d(0,0,-10)
        );


    gfc_matrix_rotate(
          modelMat3,
          modelMat3,
          M_PI/2,
          vector3d(1,0,0)
    );
    gfc_matrix_rotate(
          modelMat3,
          modelMat3,
          M_PI/2,
          vector3d(0,1,0)
    );
    gfc_matrix_rotate(
          modelMat4,
          modelMat4,
          M_PI/2,
          vector3d(1,0,0)
    );

    gfc_matrix_rotate(
          modelMat4,
          modelMat4,
          M_PI/2,
          vector3d(0,1,0)
    );



    gfc_matrix_rotate(
            modelMat5,
            modelMat5,
            M_PI/2,
            vector3d(1,0,0)
    );

   gfc_matrix_rotate(
            modelMat5,
            modelMat5,
            M_PI/2,
            vector3d(0,1,0)
    );

    player = gf3d_entity_new();
    player->model = mario;
    player->position.x = modelMat5[3][0];
    player->position.y = modelMat5[3][1];
    player->position.z = modelMat5[3][2];
    player->acceleration = vector3d(0,0,-9.8);
    player->velocity = vector3d(0,0,0);
    player->lastUpdate = 0;
    //gfc_matrix_copy(player->entityMat,modelMat5);
    player->entityMat = &modelMat5;
    //player->entityMat = modelMat5;
    gf3d_physics_set_time();
    while(!done)
    {

        SDL_PumpEvents();   // update SDL's internal event structures

        keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_UP])
        {
          gf3d_vgraphics_move_camera(vector3d(0,0,0.01));
        }
        if(keys[SDL_SCANCODE_DOWN])
        {
          gf3d_vgraphics_move_camera(vector3d(0,0,-0.01));
        }
        if(keys[SDL_SCANCODE_RIGHT])
        {
          gf3d_vgraphics_rotate_camera(.001,vector3d(0,0,1));
        }
        if(keys[SDL_SCANCODE_LEFT])
        {
          gf3d_vgraphics_rotate_camera(-.001,vector3d(0,0,1));
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

              //  gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);
              //  gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                gf3d_model_draw(model3,bufferFrame,commandBuffer,modelMat3,0);
                gf3d_model_draw(testSquare,bufferFrame,commandBuffer,modelMat4,0);
                gf3d_model_draw(mario,bufferFrame,commandBuffer,modelMat5,0);
            gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);
        gf3d_update_all_entities();
        //gf3d_physics_update(player);
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());

        //SDL_PumpEvents();
    }

    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
