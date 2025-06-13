#ifndef MESH_H
#define MESH_H

struct Mesh
{
    Vector3* vertices;
    s32 num_vertices;
    u16* indices;
    s32 num_indices;
};

#endif
