#ifndef RENDERER_H
#define RENDERER_H


struct Mesh
{
    Vector3* vertices;
    u16* indices;
};

inline u32 color_make(r32 r, r32 g, r32 b, r32 a)
{
    u32 R = (u32)Floor(r*255);
    u32 G = (u32)Floor(g*255);
    u32 B = (u32)Floor(b*255);
    u32 A = (u32)Floor(a*255);

    u32 result;

    result = ((A << 24) | (R << 16) | (G << 8) | B);
    
    return result;
    
}

#endif
