inline u32 framebuffer_pitch(u32 height = FRAME_BUFFER_HEIGHT, u32 bytpp = FRAME_BUFFER_BYTPP)
{
    return height * bytpp;
}
inline u32 framebuffer_bytesize(u32 width = FRAME_BUFFER_WIDTH, u32 height = FRAME_BUFFER_HEIGHT, u32 bytpp = FRAME_BUFFER_BYTPP)
{
    return width * height * bytpp;
}

// @doc obviously this assumes that bytpp = 4

// @doc - is placed here, so that accessing into the frame buffer
// goes from bottom to top, this way we can guarantee y is up for
// the rest of the engine, and is mapped to y is down properly here
inline u32* framebuffer_access(u32 x, u32 y, u8* buff = FRAME_BUFFER_BASE, u32 pitch = FRAME_BUFFER_PITCH, u32 bytpp = FRAME_BUFFER_BYTPP)
{
    return (u32*)(buff - y*pitch + x*bytpp);
}
inline r32* zbuffer_access(u32 x, u32 y, r32* buff = Z_BUFFER, u32 pitch = FRAME_BUFFER_WIDTH)
{
    return buff - y*pitch + x;
}


// @todo do not need to pass pitch here if I pass width and height also
inline void zbuffer_reset(r32* buff = Z_BUFFER, u32 pitch = FRAME_BUFFER_WIDTH, u32 width = FRAME_BUFFER_WIDTH, u32 height = FRAME_BUFFER_HEIGHT)
{
    for (u32 y = 0; y < height; y++)
        {
            for (u32 x = 0; x < width; x++)
                {
                    *zbuffer_access(x, y, buff, pitch) = Z_BUFFER_RESET_VALUE;
                }
        }
}
