#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    float frame = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model = NULL;
    Matrix4 modelMat;
    Sprite *mouse = NULL;
    int mousex,mousey;
    Uint32 mouseFrame = 0;
    
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
    
    model = gf3d_model_load_animated("dino_anim",5, 29);

    gfc_matrix_identity(modelMat);
    gfc_matrix_rotate(
        modelMat,
        modelMat,
        M_PI/2,
        vector3d(1,0,0));
    
    mouse = gf3d_sprite_load("images/pointer.png",32,32, 16);
    // main game loop
    slog("gf3d main loop begin");
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_GetMouseState(&mousex,&mousey);
        slog("mouse (%i,%i)",mousex,mousey);
        //update game things here
        
        gf3d_vgraphics_rotate_camera(0.001);
                frame = frame + 0.05;
                if (frame >= 24)frame = 0;
                mouseFrame = (mouseFrame+1) %16;

        // configure render command for graphics command pool
        bufferFrame = gf3d_vgraphics_render_begin();
        // for each mesh, get a command and configure it from the pool
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(),bufferFrame);
            gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(),bufferFrame);

            commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_model_pipeline());
                gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat,(Uint32)frame);
            gf3d_command_rendering_end(commandBuffer);
                
        // 2D overlay rendering
            commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_overlay_pipeline());

                gf3d_sprite_draw(mouse,vector2d(mousex,mousey),mouseFrame, bufferFrame,commandBuffer);
                
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
