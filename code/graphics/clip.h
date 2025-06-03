#ifndef CLIP_H
#define CLIP_H

typedef Vector4 Vertex4;

b32 InsideRight(Vector4 v)
{
    return v.x <= v.w;
}
b32 InsideLeft(Vector4 v)
{
    return v.x >= -v.w;
}
b32 InsideTop(Vector4 v)
{
    return v.y <= v.w;
}
b32 InsideBottom(Vector4 v)
{
    return v.y >= -v.w;
}
b32 InsideNear(Vector4 v)
{
    return v.z >= -v.w;
}
b32 InsideFar(Vector4 v)
{
    return v.z <= v.w;
}

inline Vector4 lerp(Vector4 a, Vector4 b, r32 t)
{
    Vector4 result;
    result.x = a.x + t * (b.x - a.x);
    result.y = a.y + t * (b.y - a.y);
    result.z = a.z + t * (b.z - a.z);
    result.w = a.w + t * (b.w - a.w);
    return result;
}

Vector4 IntersectRight(Vector4 a, Vector4 b)
{
    r32 t = (a.w - a.x) / ((a.w - a.x) - (b.w - b.x));
    return lerp(a, b, t);
}
Vector4 IntersectLeft(Vector4 a, Vector4 b)
{
    r32 t = (a.w + a.x) / ((a.w + a.x) - (b.w + b.x));
    return lerp(a, b, t);
}
Vector4 IntersectTop(Vector4 a, Vector4 b)
{
    r32 t = (a.w - a.y) / ((a.w - a.y) - (b.w - b.y));
    return lerp(a, b, t);
}
Vector4 IntersectBottom(Vector4 a, Vector4 b)
{
    r32 t = (a.w + a.y) / ((a.w + a.y) - (b.w + b.y));
    return lerp(a, b, t);
}
Vector4 IntersectNear(Vector4 a, Vector4 b)
{
    r32 t = (a.w + a.z) / ((a.w + a.z) - (b.w + b.z));
    return lerp(a, b, t);
}
Vector4 IntersectFar(Vector4 a, Vector4 b)
{
    r32 t = (a.w - a.z) / ((a.w - a.z) - (b.w - b.z));
    return lerp(a, b, t);
}


s32 ClipAgainstPlane(Vertex4* in_vertices, s32 in_count, Vertex4* out_vertices,
                     b32 (*Inside)(Vertex4), Vertex4 (*Intersect)(Vertex4, Vertex4))
{
    s32 out_count = 0;
    Vertex4 prev = in_vertices[in_count-1];

    for (s32 i = 0; i < in_count; i++)
    {
        Vertex4 curr = in_vertices[i];

        b32 prev_inside = Inside(prev);
        b32 curr_inside = Inside(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                out_vertices[out_count++] = Intersect(prev, curr);
            }
            out_vertices[out_count++] = curr;
        }
        else if (prev_inside)
        {
            out_vertices[out_count++] = Intersect(prev, curr);
        }

        prev = curr;
    }

    return out_count;
}

s32 ClipTriangle(TriangleHom* in_tri, TriangleHom* out_triangles)
{
    Vector4 A = in_tri->A;
    Vector4 B = in_tri->B;
    Vector4 C = in_tri->C;
    
    Vertex4 input[8];
    Vertex4 temp[8];
    Vertex4 temp2[8];

    input[0] = A;
    input[1] = B;
    input[2] = C;
    s32 count = 3;

    // Clip against each plane in order
    count = ClipAgainstPlane(input, count, temp, InsideRight, IntersectRight);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideLeft, IntersectLeft);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp2, count, temp, InsideTop, IntersectTop);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideBottom, IntersectBottom);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp2, count, temp, InsideNear, IntersectNear);
    if (count == 0) return 0;

    count = ClipAgainstPlane(temp, count, temp2, InsideFar, IntersectFar);
    if (count == 0) return 0;

    // Now temp2 holds final polygon
    // Triangulate fan (0,1,2), (0,2,3), (0,3,4), etc
    for (s32 i = 1; i < count-1; i++)
    {
        out_triangles[i-1].A = temp2[0];
        out_triangles[i-1].B = temp2[i];
        out_triangles[i-1].C = temp2[i+1];
    }

    return count-2; // Number of triangles output
}


#endif
