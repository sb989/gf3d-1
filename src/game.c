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


    Entity *floor0,*floor1,*floor2,*floor3;
    Entity *wall;
    Entity *wall2,*wall3,*wall4,*wall5,*wall6;
    Entity *pillar0,*pillar1,*pillar2;
    Entity *bar0,*bar1,*bar2,*bar3;
    Entity *player;
    Entity * toad, *goomba, *koopa;
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




    gf3d_vgraphics_move_camera(vector3d(0,0,-50));
    gf3d_vgraphics_move_camera(vector3d(0,-20,0));


    floor0 = gf3d_entity_init("floor0",true,0,1,false);
    //floor1 = gf3d_entity_init("floor1",true,0,1);
    //floor2 = gf3d_entity_init("floor2",true,0,1);
    //floor3 = gf3d_entity_init("floor3",true,0,1);
    //gf3d_entity_move(floor1,0,152,0);
    //gf3d_entity_move(floor2,-87,120,0);
    //gf3d_entity_move(floor3,-87,196,0);
    //gf3d_update_entity_bounding_box(floor1,0,0);
    //gf3d_update_entity_bounding_box(floor2,0,0);
    //gf3d_update_entity_bounding_box(floor3,0,0);
    koopa = gf3d_entity_init("koopa",false,0,1,true);
    goomba = gf3d_entity_init("goomba",false,0,1,true);
    toad = gf3d_entity_init("toad",false,0,1,true);
    gf3d_entity_move(koopa,-30,0,8);
    gf3d_entity_move(goomba,-35,5,8);
    gf3d_entity_move(toad,-40,7,8);
    gf3d_update_entity_bounding_box(toad,0,0);
    gf3d_update_entity_bounding_box(goomba,0,0);
    gf3d_update_entity_bounding_box(koopa,0,0);

    wall = gf3d_entity_init("wall",true,0,1,false);
    wall2 = gf3d_entity_init("wall2",true,0,1,false);
  //  wall3 = gf3d_entity_init("wall3",true,0,1);
    wall4 = gf3d_entity_init("wall4",true,0,1,false);
    wall5 = gf3d_entity_init("wall5",true,0,1,false);
    wall6 = gf3d_entity_init("wall6",true,0,1,false);
    gf3d_entity_move(wall,0,-(wall->height)/2,(wall->height)/2);
    gf3d_entity_move(wall2,0,(wall->height)/2,(wall->height)/2);
    slog("wall height = %f",wall->height);
  //  gf3d_entity_move(wall3,-20,185,19);
    gf3d_entity_move(wall4,35,-5,19);
    gf3d_entity_move(wall5,50,-5,25);
    gf3d_entity_move(wall6,65,-5,19);

    gf3d_update_entity_bounding_box(wall,0,0);
    gf3d_update_entity_bounding_box(wall2,0,0);
  //  gf3d_update_entity_bounding_box(wall3,0,0);
    gf3d_update_entity_bounding_box(wall4,0,0);
    gf3d_update_entity_bounding_box(wall5,0,0);
    gf3d_update_entity_bounding_box(wall6,0,0);

    pillar0 = gf3d_entity_init("pillar0",true,0,1,false);
    pillar1 = gf3d_entity_init("pillar1",true,0,1,false);
    pillar2 = gf3d_entity_init("pillar2",true,0,1,false);

    gf3d_entity_move(pillar0,0,-10,17);//-14,105,17);
    gf3d_entity_move(pillar1,-10,-10,37);//-15,118,37);
    gf3d_entity_move(pillar2,-22,-10,17);//-14,128,17);

    gf3d_update_entity_bounding_box(pillar0,0,0);
    gf3d_update_entity_bounding_box(pillar1,0,0);
    gf3d_update_entity_bounding_box(pillar2,0,0);

    bar0 = gf3d_entity_init("bar",true,0,1,false);
    bar1 = gf3d_entity_init("bar",true,0,1,false);
    bar2 = gf3d_entity_init("bar",true,0,1,false);
    bar3 = gf3d_entity_init("bar",true,0,1,false);

    gf3d_entity_move(bar0,-4,0,15);
    gf3d_entity_move(bar1,-8,0,15);
    gf3d_entity_move(bar2,-12,0,15);
    gf3d_entity_move(bar3,-16,0,15);

    gf3d_update_entity_bounding_box(bar0,0,0);
    gf3d_update_entity_bounding_box(bar1,0,0);
    gf3d_update_entity_bounding_box(bar2,0,0);
    gf3d_update_entity_bounding_box(bar3,0,0);

    gf3d_physics_set_time();
    player = gf3d_player_init();
    Bool jump = false;
    slog("starting main game loop");
    while(!done)
    {

        SDL_PumpEvents();   // update SDL's internal event structures


        //gf3d_entity_draw(player,0,bufferFrame,commandBuffer);


        keys = SDL_GetKeyboardState(NULL);
        // get the keyboard state for this frame
        //update game things here

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
        commandBuffer = gf3d_command_rendering_begin(bufferFrame);
        gf3d_entity_draw(wall,0,bufferFrame,commandBuffer);
        //gf3d_entity_draw(wall3,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(wall4,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(wall5,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(wall6,0,bufferFrame,commandBuffer);
        //gf3d_entity_draw(wall2,0,bufferFrame,commandBuffer);
        gf3d_player_draw(bufferFrame,commandBuffer);
        gf3d_entity_draw(floor0,0,bufferFrame,commandBuffer);
      //  gf3d_entity_draw(floor1,0,bufferFrame,commandBuffer);
        //gf3d_entity_draw(floor2,0,bufferFrame,commandBuffer);
      //  gf3d_entity_draw(floor3,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(goomba,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(koopa,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(toad,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(pillar0,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(pillar1,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(pillar2,0,bufferFrame,commandBuffer);

        gf3d_entity_draw(bar0,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(bar1,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(bar2,0,bufferFrame,commandBuffer);
        gf3d_entity_draw(bar3,0,bufferFrame,commandBuffer);

        gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);
        //if(player->lastUpdate == 0){
        //  player->lastUpdate = gf3d_physics_current_time();
        //}
        //gf3d_update_all_entities();
        //slog("player think");
        gf3d_player_think();
        //gf3d_player_update(player);
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
        //count++;
        //SDL_PumpEvents();
    }

    //cleanup
  //  free(floor->entityBoundingBoxes);
  //  free(player->entityBoundingBoxes);
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
