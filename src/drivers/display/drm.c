
#include "../display.h"
#include <stdio.h>

lv_display_t *display_create(void)
{
    lv_display_t *display = lv_linux_drm_create();
    if (display == NULL) {
        return NULL;
    }


    if (lv_linux_drm_set_file(display, "/dev/dri/card0", 393) //hardcoded for the time being
        != LV_RESULT_OK) {
        fprintf(stderr, "Could not initialize DRM display\n");
        lv_display_delete(display);
        return NULL;
    }

    return display;
}
