#ifndef __GF3D_UI_H__
#define __GF3D_UI_H__
#include "gf3d_texture.h"
#include "gf3d_vgraphics.h"
typedef struct
{
  VkImage             uiImage;
  VkImageView         uiImageView;
  TextLine            filename;
  Uint8               _inuse;
  Uint32              _refcount;

}UIElement;

UIElement *gf3d_ui_get_by_filename(char * filename);
void gf3d_ui_load_element();
#endif __GF3D_UI_H__
