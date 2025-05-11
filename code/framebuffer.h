#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define MAX_FRAME_BUFFER_SIZE 2073600 // 1920*1080
#define Z_BUFFER_RESET_VALUE 1.0f


struct FrameBuffer
{
    u8* base;
    u32 width;
    u32 height;
};

inline u32 framebuffer_pitch(u32 height, u32 bytpp);
inline u32 framebuffer_bytesize(u32 width, u32 height, u32 bytpp);
inline u32* framebuffer_access(u32 x, u32 y, u8* buff, u32 pitch, u32 bytpp);
inline r32* zbuffer_access(u32 x, u32 y, u8* buff, u32 pitch, u32 bytpp);
inline void zbuffer_reset(u8* buff, u32 pitch, u32 bytpp, u32 width, u32 height);



#endif
