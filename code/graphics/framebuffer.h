#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define MAX_FRAMEBUFFER_SIZE 2073600 // 1920*1080
#define Z_BUFFER_RESET_VALUE 500.0f
// #define Z_BUFFER_RESET_VALUE 1.0f

#define BYTPP 4


struct Framebuffer
{
    u8* base;
    s32 width;
    s32 height;
};

inline s32 framebuffer_pitch(s32 height);
inline s32 framebuffer_bytesize(s32 width, s32 height);
inline u32* framebuffer_access(s32 x, s32 y, u8* buff, s32 pitch);
inline r32* zbuffer_access(s32 x, s32 y, u8* buff, s32 pitch);
inline void zbuffer_reset(u8* buff, s32 pitch, s32 width, s32 height);


#endif
