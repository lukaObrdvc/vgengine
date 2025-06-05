#ifndef CLIP_H
#define CLIP_H

s32 triangulate_fan(Vector4* vertices, s32 count, Triangle4* out)
{
    for (s32 i = 1; i < count - 1; i++)
    {
        out[i-1].a = vertices[0];
        out[i-1].b = vertices[i];
        out[i-1].c = vertices[i + 1];
    }
    // triangle count is always this
    return count - 2;
}

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

// Sutherland–Hodgman polygon clipping algorithm.
// code is literally the same for every plane except for intersect
// and inside functions which differ, everything else is copy-pasted
s32 clip_triangle(Triangle4* in_tri, Triangle4* out_triangles)
{
    Scratch* scratch = get_scratch();

    Vector4* temp1 = scratch_push<Vector4>(scratch, 8);
    Vector4* temp2 = scratch_push<Vector4>(scratch, 8);

    temp1[0] = in_tri->a;
    temp1[1] = in_tri->b;
    temp1[2] = in_tri->c;
    
    s32 temp1_count = 3;
    s32 temp2_count = 0;
    
    Vector4 prev = temp1[temp1_count-1];

    // RIGHT PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_right(prev);
        b32 curr_inside = inside_right(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_right(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_right(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];

    // LEFT PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_left(prev);
        b32 curr_inside = inside_left(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_left(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_left(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];

    // TOP PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_top(prev);
        b32 curr_inside = inside_top(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_top(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_top(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];

    // BOT PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_bot(prev);
        b32 curr_inside = inside_bot(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_bot(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_bot(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];

    // NEAR PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_near(prev);
        b32 curr_inside = inside_near(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_near(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_near(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];


    // FAR PLANE
    for (s32 i = 0; i < temp1_count; i++)
    {
        Vector4 curr = temp1[i];

        b32 prev_inside = inside_far(prev);
        b32 curr_inside = inside_far(curr);

        if (curr_inside)
        {
            if (!prev_inside)
            {
                temp2[temp2_count++] = intersect_far(prev, curr);
            }
            temp2[temp2_count++] = curr;
        }
        else if (prev_inside)
        {
            temp2[temp2_count++] = intersect_far(prev, curr);
        }

        prev = curr;
    }
    if (temp2_count == 0)
    {
        release_scratch(scratch);
        return 0;
    }
    temp1_count = temp2_count;
    temp2_count = 0;
    swap(temp1, temp2);
    prev = temp1[temp1_count - 1];

    s32 triangles_count = triangulate_fan(temp1, temp1_count, out_triangles);

    release_scratch(scratch);
    return triangles_count;
}


#endif
