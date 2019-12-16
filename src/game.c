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
#include "simple_json.h"
#include "gf3d_sprite.h"


int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
  //float frame = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;

    VkCommandBuffer commandBuffer;



    //VkOffset3D (*offset)[2] = {start,end};

    Entity *floor0;
    Entity *player;

    Entity *sky;

    int mousex,mousey;
    Uint32 mouseFrame = 0;
    SJson *config;
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
        vector4d(1,1,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );

    //config = sj_load("config/main_menu.json");
  //  sj_echo(config);
    gf3d_entity_manager_init(100);
  //  SJson * temp = sj_object_new();
  //  temp = sj_object_get_value(config,"sprites");
  //  sj_echo(temp);
    // main game loop
    slog("gf3d main loop begin");




    gf3d_vgraphics_move_camera(vector3d(0,0,-30));
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

  //  sky = gf3d_entity_init("sky",true,0,1,false);

  //  gf3d_entity_move(sky,0,-(sky->height)/3,0);

  //  gf3d_update_entity_bounding_box(sky,0,0);

    gf3d_physics_set_time();
    //player = gf3d_player_init();
    Bool jump = false;
    slog("starting main game loop");




    // main game loop
    //test = gf3d_sprite_load("images/paper_mario_font_clean.png",24,24,168);

    slog("gf3d main loop begin");

    //map = gf3d_sprite_load("images/map.png",608,480,1);
    //title = gf3d_sprite_load("images/paper_mario_title.png",467,214,1);
    int frame = 0;
    Bool click = false;
    gf3d_level_load_read_config("config/main_menu.json");
    while(!done)
    {

        SDL_PumpEvents();   // update SDL's internal event structures

        //gf3d_entity_draw(player,0,bufferFrame,commandBuffer);

        //slog("numsber of swapimages is %d",sizeof(*gf3d_swapchain_get_images()));

        keys = SDL_GetKeyboardState(NULL);
        // get the keyboard state for this frame
        //update game things here

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin(); //gets image from swap chain exectues command buffer with that image returns image to swap chain
        //bufferFrame2 = gf3d_vgraphics_render_begin();

        SDL_GetMouseState(&mousex,&mousey);
        //slog("mouse (%i,%i)",mousex,mousey);
        //update game things here
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(),bufferFrame);
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(),bufferFrame);

        //commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_model_pipeline());

      //  gf3d_entity_draw(sky,0,bufferFrame,commandBuffer);

        //gf3d_player_draw(bufferFrame,commandBuffer);
        //gf3d_entity_draw(floor0,0,bufferFrame,commandBuffer);


        //gf3d_command_rendering_end_graphics(commandBuffer);

        // 2D overlay rendering
        //commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_overlay_pipeline());
        //gf3d_sprite_draw(mouse,vector2d(mousex,mousey),1,bufferFrame,commandBuffer);
        //gf3d_sprite_draw(mouse,vector2d(30,30),2,bufferFrame,commandBuffer);
        if(keys[SDL_SCANCODE_UP] && !click)
        {
          click = true;
          if(frame==167)
            frame=0;
          else
            frame = frame +1;
          slog("frame = %d",frame);
        }
        if(!keys[SDL_SCANCODE_UP])
          click = false;
/*
        gf3d_sprite_draw(mouse,vector2d(mousex,mousey),frame,bufferFrame,commandBuffer);
        gf3d_sprite_draw(red_tile,vector2d(30,30),1,bufferFrame,commandBuffer);
        gf3d_sprite_draw(h,vector2d(45,33),64,bufferFrame,commandBuffer);
        gf3d_sprite_draw(p,vector2d(65,33),72,bufferFrame,commandBuffer);
        gf3d_sprite_draw(space,vector2d(85,33),0,bufferFrame,commandBuffer);


        gf3d_sprite_draw(one,vector2d(95,33),84,bufferFrame,commandBuffer);
        gf3d_sprite_draw(zero,vector2d(115,33),83,bufferFrame,commandBuffer);
        gf3d_sprite_draw(slash,vector2d(135,33),24,bufferFrame,commandBuffer);
        gf3d_sprite_draw(one2,vector2d(145,33),84,bufferFrame,commandBuffer);
        gf3d_sprite_draw(zero2,vector2d(165,33),83,bufferFrame,commandBuffer);
        */
        //gf3d_sprite_draw(map,vector2d(0,0),0,bufferFrame,commandBuffer);
        //gf3d_sprite_draw(title,vector2d(30,30),0,bufferFrame,commandBuffer);

        done = gf3d_level_load_draw_all(bufferFrame);
        //gf3d_sprite_draw(mouse,vector2d(mousex,mousey),frame,bufferFrame,commandBuffer);
        //gf3d_sprite_draw(mouse,vector2d(mousex,mousey),mouseFrame, bufferFrame,commandBuffer2);
        //gf3d_command_rendering_end_graphics(commandBuffer);


        //gf3d_command_rendering_end_graphics(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);

        //bufferFrame = gf3d_vgraphics_render_begin();


        //j
        //gf3d_vgraphics_render_end(uiBufferFrame);
        //if(player->lastUpdate == 0){
        //  player->lastUpdate = gf3d_physics_current_time();
        //}
        //gf3d_update_all_entities();
        //slog("player think");

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
