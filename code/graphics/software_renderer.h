#ifndef SOFTWARE_RENDERER_H
#define SOFTWARE_RENDERER_H

union Triangle
{
    struct { Vector3 a, b, c; };
    Vector3 v[3];
};

union Triangle4
{
    struct { Vector4 a, b, c; };
    Vector4 v[3];
};

// @todo probably make a function that changes winding?

inline void triangle_4to3(Triangle4* t4, Triangle* t3)
{
    t3->a = vec_4to3(t4->a);
    t3->b = vec_4to3(t4->b);
    t3->c = vec_4to3(t4->c);
}

inline Triangle* ttriangle_4to3(Triangle4* t)
{
    Triangle* tri = temp_alloc(Triangle);
    triangle_4to3(t, tri);
    return tri;
}

void fill_background();
s32 triangulate_fan(Vector4* vertices, s32 count, Triangle4* out);
s32 clip_triangle(Triangle4* in_tri, Triangle4* out_triangles);
void rasterize_triangle(Triangle* tri, Color color, b32 inv);
void triangle_clip_to_raster_space(Triangle4* t);
void render_mesh(Mesh mesh, Matrix4* mvp, Color* colors);
Color bilinear_filtering(const Color& src, const Color& dest);

inline b32 inside_right(Vector4 v)
{
    return v.x <= v.w;
}
inline b32 inside_left(Vector4 v)
{
    return v.x >= -v.w;
}
inline b32 inside_top(Vector4 v)
{
    return v.y <= v.w;
}
inline b32 inside_bot(Vector4 v)
{
    return v.y >= -v.w;
}
inline b32 inside_near(Vector4 v)
{
    return v.z >= -v.w;
}
inline b32 inside_far(Vector4 v)
{
    return v.z <= v.w;
}

inline Vector4 intersect_right(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w - a.x) / ((a.w - a.x) - (b.w - b.x));
    return vec_lerp(a, b, t);
}
inline Vector4 intersect_left(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w + a.x) / ((a.w + a.x) - (b.w + b.x));
    return vec_lerp(a, b, t);
}
inline Vector4 intersect_top(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w - a.y) / ((a.w - a.y) - (b.w - b.y));
    return vec_lerp(a, b, t);
}
inline Vector4 intersect_bot(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w + a.y) / ((a.w + a.y) - (b.w + b.y));
    return vec_lerp(a, b, t);
}
inline Vector4 intersect_near(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w + a.z) / ((a.w + a.z) - (b.w + b.z));
    return vec_lerp(a, b, t);
}
inline Vector4 intersect_far(const Vector4& a, const Vector4& b)
{
    r32 t = (a.w - a.z) / ((a.w - a.z) - (b.w - b.z));
    return vec_lerp(a, b, t);
}

inline r32 edge_function(const Vector3& c, const Vector3& b, const Vector3& a)
{
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}

inline Rect triangle_bounding_box(const Vector3& a, const Vector3& b, const Vector3& c)
{
    Rect result;
    
    result.min_x = Min(Min(a.x, b.x), c.x);
    result.min_y = Min(Min(a.y, b.y), c.y);
    result.max_x = Max(Max(a.x, b.x), c.x);
    result.max_y = Max(Max(a.y, b.y), c.y);

    return result;
}

#endif
