#ifndef RENDERER_H
#define RENDERER_H


// @NOTE there is no correlation between vertices and indices,
// only some min/max constraint that is not very useful (maybe can check
// just in case); this means we have a variable size array of v3's and
// a variable size array of u16's
typedef struct tagMesh
{
    v3* vertices;
    u16* indices;
} Mesh;

// we can say that if clipping->triangulation leads to additional geometry,
// append triangles to end of mesh arrays, and stop rasterization of
// triangle;

// you definately do not want to put extra geometry directly into the mesh,
// because then it loses correct information; you should put extra geometry
// someplace else, then say you always render it, but then if you don't
// render some of it once in a frame, you remove that geometry;

// but the issue here is that once you get to triangles that are invalid,
// because they have been clipped, you have to skip rendering that one,
// and here you have extra information, but you retain the original mesh
// information

// the problem is you have to also check every frame whether invalid
// triangles are valid again



// so here's the solution:
// - every frame do clipping
//   - if triangle fully clipped then skip everything
//   - if triangle fully in viewport, then draw it whole
//   - if triangle clipped, triangulate the clipped polygon, put that
//     extra geometry somewhere else (this should be a dynarr) and render
//     it later, and you don't do clipping and triangulation on those
//     (but do you even do perspective divide, camera transform and so on?,
//      why not render it immediately ? since you will do the following
//      anyway) (the problem is that you don't know how much geometry
//      will be generated so you have to have a dynarr to put it in; but
//      you will know the maximum and minimum geometry, so you can just
//      have a static small buffer that keeps that as extra geoemtry is
//      generated and render it immediately, and then empty dynarr fully)
//   - after rendering all extra geometry, empty that memory fully and
//     next frame just do everything again


inline u32 color_make(float r, float g, float b, float a)
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
