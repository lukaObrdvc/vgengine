inline s32 framebuffer_pitch(s32 width = FRAMEBUFFER_WIDTH)
{
    return width * BYTPP;
}
inline s32 framebuffer_bytesize(s32 width = FRAMEBUFFER_WIDTH, s32 height = FRAMEBUFFER_HEIGHT)
{
    return width * height * BYTPP;
}

// @doc - is placed here, so that accessing into the frame buffer
// goes from bottom to top, this way we can guarantee y is up for
// the rest of the engine, and is mapped to y is down properly here
inline u32* framebuffer_access(s32 x, s32 y, u8* buff = FRAMEBUFFER_BASE, s32 pitch = FRAMEBUFFER_PITCH)
{
    return (u32*)(buff - y * pitch + x * BYTPP);
}
inline r32* zbuffer_access(s32 x, s32 y, r32* buff = ZBUFFER, s32 pitch = FRAMEBUFFER_WIDTH)
{
    return buff - y * pitch + x;
}

inline void zbuffer_reset(r32* buff = ZBUFFER, s32 width = FRAMEBUFFER_WIDTH, s32 height = FRAMEBUFFER_HEIGHT)
{
    for (s32 y = 0; y < height; y++)
    {
        for (s32 x = 0; x < width; x++)
        {
            *zbuffer_access(x, y, buff, width) = Z_BUFFER_RESET_VALUE;
        }
    }
}
