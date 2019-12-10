#include "gf3d_ui.h"

typedef struct
{

  UIElement * ui_elements_list;
  uint32_t max_ui_elements;
}UIManager;

static UIManager gf3d_ui_manager = {0};

void gf3d_ui_load_element()
{

}

UIElement *gf3d_ui_get_by_filename(char * filename)
{
    int i;
    if (!filename)return NULL;
    for (i = 0; i < gf3d_ui_manager.max_ui_elements; i++)
    {
        if (!gf3d_ui_manager.ui_elements_list[i]._inuse)continue;
        if (gfc_line_cmp(gf3d_ui_manager.ui_elements_list[i].filename,filename)==0)
        {
            return &gf3d_ui_manager.ui_elements_list[i];
        }
    }
    return NULL;
}
