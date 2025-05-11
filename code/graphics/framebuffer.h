#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#define MAX_FRAMEBUFFER_SIZE 2073600 // 1920*1080


struct FrameBuffer
{
    u8* base;
    u32 width;
    u32 height;
};

inline FrameBuffer get_frame_buffer()
{
    return get_engine_state()->frameBuffer;
}
inline u8* get_frame_buffer_base()
{
    return get_engine_state()->frameBuffer.base;
}
inline u32 get_frame_buffer_width()
{
    return get_engine_state()->frameBuffer.width;
}
inline u32 get_frame_buffer_height()
{
    return get_engine_state()->frameBuffer.height;
}
inline u32 get_frame_buffer_bytpp()
{
    return get_platform_api().platformDisplay.bytesPerPixel;
}

// @doc - is placed here, so that accessing into the frame buffer
// goes from bottom to top, this way we can guarantee y is up for
// the rest of the engine, and is mapped to y is down properly here
inline u32 get_frame_buffer_pitch()
{
    return - get_frame_buffer_height() * get_frame_buffer_bytpp();
}
inline u32 get_frame_buffer_bytesize()
{
    return get_frame_buffer_width() * get_frame_buffer_height() * get_frame_buffer_bytpp();
}

inline u32 framebuffer_pitch(u32 height, u32 bytpp)
{
    return height * bytpp;
}
inline u32 framebuffer_bytesize(u32 width, u32 height, u32 bytpp)
{
    return width * height * bytpp;
}

inline u8* get_z_buffer()
{
    return get_engine_state()->zBuffer;
}

// @todo maybe make overloading versions with default stuff for this
// so you don't have to pass all of this stuff all the time, but idk..?
// @doc obviously this assumes that bytpp = 4
inline u32* framebuffer_access(u8* buff, u32 pitch, u32 bytpp, u32 x, u32 y)
{
    return (u32*)(buff + y*pitch + x*bytpp);
}
inline r32* zbuffer_access(u8* buff, u32 pitch, u32 bytpp, u32 x, u32 y)
{
    return (r32*)(buff + y*pitch + x*bytpp);
}

inline void zbuffer_reset(u8* buff, u32 pitch, u32 bytpp, u32 width, u32 height);
{
    for (u32 y = 0; y < height; y++)
        {
            for (u32 x = 0; x < width; x++)
                {
                    *zbuffer_access(buff, pitch, bytpp, x, y) = 1.0f; // @todo think about this 1.0f ....
                }
        }
}




#endif
