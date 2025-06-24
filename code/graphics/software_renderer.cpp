// @todo pass color instead of hardcoding a color, and have a default parametera
void fill_background()
{
    for (s32 y = 0; y < FRAMEBUFFER_HEIGHT; y++)
    {
        for (s32 x = 0; x < FRAMEBUFFER_WIDTH; x++)
        {
            *framebuffer_access(x, y) = ((u32)255 << 24) | ((u32)125 << 16) | ((u32)0 << 8) | ((u32)125);
        }
    }
}

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

void triangle_clip_to_raster_space(Triangle4* t)
{
    // perspective divide is done here
    t->a = vec_scale(t->a, 1/t->a.w);
    t->b = vec_scale(t->b, 1/t->b.w);
    t->c = vec_scale(t->c, 1/t->c.w);
    
    s32 width = FRAMEBUFFER_WIDTH;
    s32 height = FRAMEBUFFER_HEIGHT;
    
    // @todo figure out what is z exactly in raster space??
    t->a.x = (t->a.x + 1)/2 * width;
    t->a.y = (t->a.y + 1)/2 * height;

    t->b.x = (t->b.x + 1)/2 * width;
    t->b.y = (t->b.y + 1)/2 * height;

    t->c.x = (t->c.x + 1)/2 * width;
    t->c.y = (t->c.y + 1)/2 * height;
}

// @todo this should actually just return a bit_array mask and a rect
// bounding the triangle so that you have information about which
// pixel should be drawn in this rect

// anti-aliasing
// top-left rule?
// figure out if we want to allow passing information about how to do culling
void rasterize_triangle(Triangle* tri, Color color, b32 inv)
{
    Vector3 p0 = tri->a;
    Vector3 p1 = tri->b;
    Vector3 p2 = tri->c;
    
    Rect bbox = triangle_bounding_box(p0, p1, p2);
    r32 area = edge_function(p2, p1, p0);
    
    for (s32 j = Floor(bbox.min_y); j <= Floor(bbox.max_y); j++)
    {
        for (s32 i = Floor(bbox.min_x); i <= Floor(bbox.max_x); i++)
        {
            Vector3 p = vec_make(i+0.5f, j+0.5f, 0.0f);
            r32 w0 = edge_function(p, p2, p1);
            r32 w1 = edge_function(p, p0, p2);
            r32 w2 = edge_function(p, p1, p0);

            // @todo figure out why i'm actually culling here when I shouldn't be??
            b32 cond = (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
            // if (w0 >= 0 && w1 >= 0 && w2 >= 0) // inside test original
            // if (w0 <= 0 && w1 <= 0 && w2 <= 0) // inside test
            if (cond)
            {
                w0 /= area; // these are barycentric coords
                w1 /= area;
                w2 /= area;

                // perspective-correct interpolation
                r32 z = 1 / (w0 / p0.z + w1 / p1.z + w2/ p2.z);
                r32* zbuffer_point = zbuffer_access(i, j);

                
                u32 r = (u32)(w0 * 255);
                u32 g = (u32)(w1 * 255);
                u32 b = (u32)(w2 * 255);

                u32 int_color = (((u32)255 << 24) | (r << 16) | (g << 8) | b);
                            
                // if (z >= *zbuffer_point) // original
                if (z < *zbuffer_point)
                {
                    *framebuffer_access(i, j) = (inv ? int_color : color_to_u32(color));
                    *zbuffer_point = z;
                }
            }
        }
    }
}

// we don't have shading yet so pass one solid color per triangle
// also for array sizes, there might be a better solution..?
void render_mesh(Mesh mesh, Matrix4* mvp, Color* colors)
{
    u64 temp_old_size = TEMPORARY_ARENA->size;
    
    // clipping of a triangle in the worst case results in 4 triangles needed to draw
    Triangle4* clipped_geometry = temp_alloc(Triangle4, 4);
    Vector4* homogeneous_vertices = temp_alloc(Vector4, mesh.num_vertices);

    for (s32 i = 0; i < mesh.num_vertices; i++)
    {
        homogeneous_vertices[i] = vec_3to4(mesh.vertices[i], 1.0f);
    }
    
    for (s32 i = 0; i < mesh.num_vertices; i++)
    {
        homogeneous_vertices[i] = matrix_mul_vector4(mvp, homogeneous_vertices[i]);
    }

    s32 c = 0; // colors index; goes to num_triangles implicitly
    for (s32 i = 0; i < mesh.num_indices; i += 3)
    {
        Triangle4 unclipped_triangle = {
            homogeneous_vertices[mesh.indices[i]],
            homogeneous_vertices[mesh.indices[i+1]],
            homogeneous_vertices[mesh.indices[i+2]]
        };

        s32 count = clip_triangle(&unclipped_triangle, clipped_geometry);
        
        for (s32 j = 0; j < count; j++)
        {
            Triangle4* clipped_triangle = &clipped_geometry[j];
            triangle_clip_to_raster_space(clipped_triangle);
            
            Triangle raster_triangle;
            triangle_4to3(clipped_triangle, &raster_triangle);

            Color color = colors[c];
            rasterize_triangle(&raster_triangle, color, false);
        }
        
        c++;
    }

    temp_set_size(temp_old_size);
}

// @doc stride is mainly for sampling a sub-texture within a texture (a rect in it), so
// this way you can step to the next texel correctly
Color bilinear_sample_premultiplied(u8* tex, s32 tex_w, s32 tex_h, s32 stride, r32 u, r32 v)
{
    s32 u0 = clamp(floori(u), 0, tex_w - 1);
    s32 v0 = clamp(floori(v), 0, tex_h - 1);
    s32 u1 = clamp(u0 + 1, 0, tex_w - 1);
    s32 v1 = clamp(v0 + 1, 0, tex_h - 1);

    r32 tu = decimal(u);
    r32 tv = decimal(v);
    
    Color c00 = u32_to_color(*(u32*)(tex + v0 * stride + u0 * BYTPP));
    Color c10 = u32_to_color(*(u32*)(tex + v0 * stride + u1 * BYTPP));
    Color c01 = u32_to_color(*(u32*)(tex + v1 * stride + u0 * BYTPP));
    Color c11 = u32_to_color(*(u32*)(tex + v1 * stride + u1 * BYTPP));

    Color top = color_lerp(c00, c10, tu);
    Color bottom = color_lerp(c01, c11, tu);
    Color result = color_lerp(top, bottom, tv);
    
    return result;
}
