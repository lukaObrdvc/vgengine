#ifndef UPDATE_AND_RENDER_H
#define UPDATE_AND_RENDER_H

struct Engine_frame_result
{
    b32 show_cursor;
};


extern "C" Engine_frame_result update_and_render();

#endif
