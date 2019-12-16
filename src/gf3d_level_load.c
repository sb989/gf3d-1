#include "gf3d_level_load.h"
#include "gf3d_entity.h"
#include "gf3d_player.h"

typedef struct
{
  List *sprites;
  List *models;
  List *music;

}LevelAssets;

typedef struct
{
  Sprite * s;
  int x_pos;
  int y_pos;
  int width;
  int height;
  int visible;
  int frame;
  int frame_num;
  int command;
  int type;
  char * level;
}SpriteInfo;

typedef struct
{
  Entity *e;
  int isPlayer;
  char * name;
}ModelInfo;

static LevelAssets lvlassets = {0};

SpriteInfo * gf3d_level_load_sprite_info_new();
ModelInfo * gf3d_level_load_model_info_new();
char * gf3d_level_load_remove_quotes(char * word);
char * gf3d_level_load_get_object_value_as_string(SJson *object,char * key);
int gf3d_level_load_draw_sprites(uint32_t bufferFrame,VkCommandBuffer commandBuffer);

SpriteInfo * gf3d_level_load_sprite_info_new()
{
  SpriteInfo *sp;
  sp = (SpriteInfo *)malloc(sizeof(SpriteInfo));
  if (!sp)
  {
      slog("failed to allocate memory for spriteinfo");
      return NULL;
  }

  return sp;
}

ModelInfo * gf3d_level_load_model_info_new()
{
  ModelInfo *m;
  m = (ModelInfo*)malloc(sizeof(ModelInfo));
  if(!m)
  {
    slog("failed to allocate memeory for modelinfo");
    return NULL;
  }
  return m;
}


void gf3d_level_load_read_config(const char * filename)
{
  SJson *config,* temp;
  //slog("setting up lvlassets");
  lvlassets.sprites = gfc_list_new();
  lvlassets.models  = gfc_list_new();
  lvlassets.music  = gfc_list_new();

  //slog("about to load json file");
  config = sj_load(filename);
  //sj_echo(config);
  if (!config)
  {
      slog("json failed to load");
  }
//  slog("successfully loaded json");
  temp = sj_object_new();

  temp = sj_object_get_value(config,"models");
  if(temp)
  {

    gf3d_level_load_model_init(temp);
  }
  slog("finished loading models");


  temp = sj_object_new();

  temp = sj_object_get_value(config,"sprites");
  //sj_echo(temp);
  if(temp)
  {
    //slog("about to load sprites");
    gf3d_level_load_sprite_init(temp);
  }
  slog("finished loading sprites");


}

void gf3d_level_load_sprite_init(SJson *sprites)
{
  int count;
  int i;
  char *file_loc,*pos_x,* pos_y, *x_dim,* y_dim,*layer_count,*visible,*frame,*num,*type,*command,*height,*width;
  SJson *value;
  SpriteInfo * si;
  Sprite * temp = NULL;
  SJString *output;
  SJson *temp_value;
  count = sj_array_get_count(sprites);
  //slog("count is %d",count);
  for (i = 0; i < count; i++)
  {
      value = sj_array_get_nth(sprites,i);
      //sj_echo(value);
      if (!value)continue;
      //slog("got value\n");
      file_loc = gf3d_level_load_get_object_value_as_string(value,"file_location");
      type = gf3d_level_load_get_object_value_as_string(value,"type");
      //slog("file_loc is %s",file_loc);
      pos_x = gf3d_level_load_get_object_value_as_string(value,"pos_x");
      //slog("pos_x is %s",pos_x);
      pos_y = gf3d_level_load_get_object_value_as_string(value,"pos_y");
      //slog("pos_y is %s",pos_y);
      x_dim = gf3d_level_load_get_object_value_as_string(value,"x_dim");
      //slog("x_dim is %s",x_dim);
      y_dim = gf3d_level_load_get_object_value_as_string(value,"y_dim");
      //slog("y_dim is %s",y_dim);
      //slog("all stats %s %s %s %s",pos_x,pos_y,x_dim,y_dim);
      layer_count = gf3d_level_load_get_object_value_as_string(value,"layer_count");
      visible = gf3d_level_load_get_object_value_as_string(value,"visible");
      frame = gf3d_level_load_get_object_value_as_string(value,"frame");
      num = gf3d_level_load_get_object_value_as_string(value,"num");
      //slog("got all the variables");
      temp = gf3d_sprite_load(file_loc,atoi(x_dim),atoi(y_dim),atoi(layer_count));
      si = gf3d_level_load_sprite_info_new();
      if(strcmp(frame,"default") == 0) si->frame = 0;
      else if(strcmp(frame,"mario_Chealth_0") == 0)  si->frame =1; //0 is the least sig digit 1 is the most sig digit
      else if(strcmp(frame,"mario_Mhealth_0")== 0) si->frame =2;  //0 is the least sig digit 1 is the most sig digit
      else if(strcmp(frame,"mario_Chealth_1")== 0)  si->frame =3; //0 is the least sig digit 1 is the most sig digit
      else if(strcmp(frame,"mario_Mhealth_1")== 0) si->frame =4;  //0 is the least sig digit 1 is the most sig digit
      else if(strcmp(frame,"alpha")== 0)  si->frame = 5;

      si->type = 0;
      if(!strcmp(type,"button"))
      {
        command = gf3d_level_load_get_object_value_as_string(value,"command");
        height = gf3d_level_load_get_object_value_as_string(value,"height");
        width = gf3d_level_load_get_object_value_as_string(value,"width");
        si->width = atoi(width);
        si->height = atoi(height);

        si->command = atoi(command);
        if(si->command)
        {
          si->level = gf3d_level_load_get_object_value_as_string(value,"level");

        }

        si->type = 1;
      }
      si->s = temp;
      si->x_pos = atoi(pos_x);
      si->y_pos = atoi(pos_y);
      si->visible = atoi(visible);
      si->frame_num = atoi(num);

      //slog("created sprite info");
      lvlassets.sprites = gfc_list_append(lvlassets.sprites,si);

  }
}

void gf3d_level_load_model_init(SJson * models)
{
  int i,count;
  SJson *value;
  char *isEnvironment,*isEnemy,*isPlayer,*startx,*starty,*startz,*startframe,*endframe,*name;
  ModelInfo * mi;
  Entity *e;
  count = sj_array_get_count(models);
  for (i = 0; i < count; i++)
  {
      value = sj_array_get_nth(models,i);
      if (!value)continue;
      isPlayer = gf3d_level_load_get_object_value_as_string(value,"isPlayer");
      //slog("isplayer is %s",isPlayer);
      mi = gf3d_level_load_model_info_new();
      e = gf3d_entity_new();
      if(atoi(isPlayer))
      {
        mi->e = gf3d_player_init();
        mi->isPlayer = 1;
        mi->name = "mario";
        e = gf3d_player_init();
        lvlassets.models = gfc_list_append(lvlassets.models,e);
      }
      else
      {
        isEnvironment = gf3d_level_load_get_object_value_as_string(value,"isEnvironment");
        isEnemy = gf3d_level_load_get_object_value_as_string(value,"isEnemy");
        startx = gf3d_level_load_get_object_value_as_string(value,"startx");
        starty = gf3d_level_load_get_object_value_as_string(value,"starty");
        startz = gf3d_level_load_get_object_value_as_string(value,"startz");
        startframe = gf3d_level_load_get_object_value_as_string(value,"startFrame");
        endframe = gf3d_level_load_get_object_value_as_string(value,"endFrame");
        name = gf3d_level_load_get_object_value_as_string(value,"name");
        slog("name is %s isEnvironment is %s startframe is %s endframe is %s isEnemy is %s",name,isEnvironment,startframe,endframe,isEnemy);
        e = gf3d_entity_init(name,atoi(isEnvironment),atoi(startframe),atoi(endframe),atoi(isEnemy));
        slog("name in entity is %s",e->name);
        gf3d_entity_move(e,atoi(startx),atoi(starty),atoi(startz));
        gf3d_update_entity_bounding_box(e,0,0);
        mi->isPlayer = 0;
        mi->name = name;
        lvlassets.models = gfc_list_append(lvlassets.models,e);
      }

  }

}

void gf3d_level_load_music_init()
{

}
int gf3d_level_load_draw_sprites(uint32_t bufferFrame,VkCommandBuffer commandBuffer)
{
  int i,frame;
  SpriteInfo * si;
  int mousex,mousey;
  SDL_GetMouseState(&mousex,&mousey);
  SDL_Event e;
  SDL_PollEvent(&e);
  for(i = 0;i<gfc_list_get_count(lvlassets.sprites);i++)
  {
    si = gf3d_level_load_sprite_info_new();
    si = gfc_list_get_nth(lvlassets.sprites,i);
    if(si->visible == 1 && si->type == 0)
    {

      switch(si->frame)
      {
        case 0:
          frame = 0;
          break;
        case 1:
          frame = (gf3d_player_get_current_health()%10)+83;
          break;
        case 2:
          frame = (gf3d_player_get_max_health()%10)+83;
          break;
        case 3:
          frame = (gf3d_player_get_current_health()/10)+83;
          break;
        case 4:
          frame = (gf3d_player_get_max_health()/10)+83;
          break;
        case 5:
          frame = si->frame_num;
          //slog("frame is %d",frame);
          break;
      }

      gf3d_sprite_draw(si->s,vector2d(si->x_pos,si->y_pos),frame,bufferFrame,commandBuffer);

    }
    else
    {
      int maxx = si->x_pos+si->width;
      int maxy = si->y_pos+si->height;
      //slog("mousex is %d and mousey is %d",mousex,mousey);
      //slog("minx is %d miny is %d maxx is %d maxy is %d",si->x_pos,si->y_pos,maxx,maxy);
      if(mousex > si->x_pos && mousex<maxx && mousey>si->y_pos && mousey<maxy )
      {

        gf3d_sprite_draw(si->s,vector2d(si->x_pos,si->y_pos),0,bufferFrame,commandBuffer);
        switch(e.type){
            case SDL_MOUSEBUTTONDOWN:
                if(si->command == 0)
                  return 1;
                if(si->command ==1)
                {
                  slog("trying to load level %s",si->level);
                  gf3d_level_load_read_config(si->level);
                }
                break;

        }

      }
    }
  }
  return 0;
}

void gf3d_level_load_draw_models(uint32_t bufferFrame,VkCommandBuffer commandBuffer)
{
  int i;
  ModelInfo * mi;
  Entity *e;
  for(i = 0;i<gfc_list_get_count(lvlassets.models);i++)
  {
    //mi = gf3d_level_load_model_info_new();
    e = gfc_list_get_nth(lvlassets.models,i);
    slog("about to draw stuff");
    if(!e)
    {
      slog("entity is empty for some reason");
    }
    slog("potatoes");
    slog("entity health is %d",e->health);
    if(strcmp(e->name,"mario") == 0)
    {
      gf3d_player_draw(bufferFrame,commandBuffer);
      gf3d_player_think();
    }
    
    else
    {
      gf3d_entity_draw(e,0,bufferFrame,commandBuffer);
    }
  }
}
int gf3d_level_load_draw_all(uint32_t bufferFrame)
{
  VkCommandBuffer commandBuffer;
  int ret;
  int mousex,mousey;
  //Sprite *mouse = gf3d_sprite_load("images/pointer.png",32,32, 16);
  SDL_GetMouseState(&mousex,&mousey);
  //mouse = gf3d_sprite_load("images/pointer.png",32,32, 16);
  commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_model_pipeline());
  gf3d_level_load_draw_models(bufferFrame,commandBuffer);
  gf3d_command_rendering_end_graphics(commandBuffer);
  commandBuffer = gf3d_command_rendering_begin(bufferFrame,gf3d_vgraphics_get_graphics_overlay_pipeline());
  //gf3d_sprite_draw(mouse,vector2d(mousex,mousey),0,bufferFrame,commandBuffer);
  ret = gf3d_level_load_draw_sprites(bufferFrame,commandBuffer);
  gf3d_command_rendering_end_graphics(commandBuffer);
  return ret;
}

char * gf3d_level_load_get_object_value_as_string(SJson *object,char * key)
{
  SJString *output;
  SJson *temp_value;
  char * temp;
  //sj_echo(object);
  slog("key is %s",key);
  temp_value = sj_object_get_value(object,key);
  sj_echo(temp_value);
  output = temp_value->get_string(temp_value);
  temp = output->text;

  temp = gf3d_level_load_remove_quotes(temp);
  //slog("%s",temp);
  sj_string_free(output);
  return temp;
}

char * gf3d_level_load_remove_quotes(char * word)
{
  char * temp;
  int i;
  temp = malloc(strlen(word));
  //slog("length is %d",strlen(word));
  /*for(i = 0;i<strlen(word);i++)
  {

    slog("%d is %s",i,word+i);
  }*/
  for(i = 0;i<strlen(word)-2;i++)
  {

    temp[i] = word[i+1];
  }
  temp[i] = '\0';

  return temp;
}
