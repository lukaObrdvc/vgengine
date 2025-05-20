inline s32 framebuffer_pitch(s32 height = FRAMEBUFFER_HEIGHT, s32 bytpp = FRAMEBUFFER_BYTPP)
{
    return height * bytpp;
}
inline s32 framebuffer_bytesize(s32 width = FRAMEBUFFER_WIDTH, s32 height = FRAMEBUFFER_HEIGHT, s32 bytpp = FRAMEBUFFER_BYTPP)
{
    return width * height * bytpp;
}

// @doc obviously this assumes that bytpp = 4

// @doc - is placed here, so that accessing into the frame buffer
// goes from bottom to top, this way we can guarantee y is up for
// the rest of the engine, and is mapped to y is down properly here
inline s32* framebuffer_access(s32 x, s32 y, u8* buff = FRAMEBUFFER_BASE, s32 pitch = FRAMEBUFFER_PITCH, s32 bytpp = FRAMEBUFFER_BYTPP)
{
    return (s32*)(buff - y*pitch + x*bytpp);
}
inline r32* zbuffer_access(s32 x, s32 y, r32* buff = ZBUFFER, s32 pitch = FRAMEBUFFER_WIDTH)
{
    return buff - y*pitch + x;
}


// @todo do not need to pass pitch here if I pass width and height also
inline void zbuffer_reset(r32* buff = ZBUFFER, s32 pitch = FRAMEBUFFER_WIDTH, s32 width = FRAMEBUFFER_WIDTH, s32 height = FRAMEBUFFER_HEIGHT)
{
    for (s32 y = 0; y < height; y++)
    {
        for (s32 x = 0; x < width; x++)
        {
            *zbuffer_access(x, y, buff, pitch) = Z_BUFFER_RESET_VALUE;
        }
    }
}
