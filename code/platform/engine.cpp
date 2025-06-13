// possibilities...:

// @Note this is probably a bad idea
/* #define normalize(n, max) ((n)*1.0f/(max)*1.0f) */
/* #define renormalize(n, max) ((n)*(max)) */


// @TODO figure out what you actually want from these from real examples
// once you have some, until then don't use these
/* inline degree moduo_degree(degree angle) */
/* { */
/*     return (r32)(((s32)angle)%DEGREE_MAX) + angle-(s32)angle; */
/* } */

/* inline rad moduo_rad(rad angle) */
/* { */
/*     return (r32)(((s32)(angle/pi))%((s32)(RAD_MAX/pi))) + angle-(s32)angle; */
/* } */


// inline r32 slope2(v2 vec1, v2 vec2)
// {
    // r32 result;

    // result = (vec1.y - vec2.y) / (vec1.x / vec2.x);
    
    // return result;
// }

inline r32 edge_function(const Vector3& c, const Vector3& b, const Vector3& a)
{
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}

// @todo maybe don't pass Triangle* ?
Rect triangle_bounding_box(Triangle* tri)
{
    Rect result;

    Vector3 A = tri->a;
    Vector3 B = tri->b;
    Vector3 C = tri->c;
    
    result.min_x = Min(Min(A.x, B.x), C.x);
    result.min_y = Min(Min(A.y, B.y), C.y);
    result.max_x = Max(Max(A.x, B.x), C.x);
    result.max_y = Max(Max(A.y, B.y), C.y);

    return result;
}

// top-left rule?
// figure out if we want to allow passing information about how to do culling
void rasterize_triangle(Triangle* tri, u32 color, b32 inv)
{
    Vector3 p0 = tri->a;
    Vector3 p1 = tri->b;
    Vector3 p2 = tri->c;
    
    Rect bbox = triangle_bounding_box(tri);
    r32 area = edge_function(p2, p1, p0);
    
    for (s32 j = Floor(bbox.min_y); j <= Floor(bbox.max_y); j++)
    {
        for (s32 i = Floor(bbox.min_x); i <= Floor(bbox.max_x); i++)
        {
            Vector3 p = vec_make(i+0.5f, j+0.5f, 0.0f);
            r32 w0 = edge_function(p, p2, p1);
            r32 w1 = edge_function(p, p0, p2);
            r32 w2 = edge_function(p, p1, p0);

            b32 cond = (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
            // if (w0 >= 0 && w1 >= 0 && w2 >= 0) // inside test original
            // if (w0 <= 0 && w1 <= 0 && w2 <= 0) // inside test
            if (cond)
            {
                w0 /= area; // these are barycentric coords
                w1 /= area;
                w2 /= area;

                // perspective-correct interpolation
                r32 z = 1/(w0/p0.z+w1/p1.z+w2/p2.z);
                r32* zbuffer_point = zbuffer_access(i, j);

                u32 r = (u32)(w0*255);
                u32 g = (u32)(w1*255);
                u32 b = (u32)(w2*255);

                u32 intColor = (((u32)255 << 24) | (r << 16) | (g << 8) | b);
                            
                // if (z >= *zbuffer_point) // original
                if (z < *zbuffer_point)
                {
                    *framebuffer_access(i, j) = (inv ? intColor : color);
                    *zbuffer_point = z;
                }
            }
        }
    }
}

void triangle_clip_to_raster_space(Triangle4* t)
{
    // perspective divide is done here
    t->a = vec_scale(t->a, 1/t->a.w);
    t->b = vec_scale(t->b, 1/t->b.w);
    t->c = vec_scale(t->c, 1/t->c.w);
    
    s32 width = FRAMEBUFFER_WIDTH;
    s32 height = FRAMEBUFFER_HEIGHT;
    
    t->a.x = (t->a.x + 1)/2 * width;
    t->a.y = (t->a.y + 1)/2 * height;

    t->b.x = (t->b.x + 1)/2 * width;
    t->b.y = (t->b.y + 1)/2 * height;

    t->c.x = (t->c.x + 1)/2 * width;
    t->c.y = (t->c.y + 1)/2 * height;
}

// we don't have shading yet so pass one solid color per triangle
// also for array sizes, there might be a better solution..?
void render_mesh(Mesh mesh, Matrix4* mvp, u32* colors, s32 num_vertices, s32 num_indices)
{
    u64 temp_old_size = TEMPORARY_ARENA->size;
    
    // clipping of a triangle in the worst case results in 4 triangles needed to draw
    Triangle4* clipped_geometry = temp_alloc(Triangle4, 4);
    Vector4* homogeneous_vertices = temp_alloc(Vector4, num_vertices);

    for (s32 i = 0; i < num_vertices; i++)
    {
        homogeneous_vertices[i] = vec_3to4(mesh.vertices[i], 1.0f);
    }
    
    for (s32 i = 0; i < num_vertices; i++)
    {
        homogeneous_vertices[i] = matrix_mul_vector4(mvp, homogeneous_vertices[i]);
    }

    s32 c = 0; // colors index; goes to num_triangles implicitly
    for (s32 i = 0; i < num_indices; i += 3)
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

            u32 color = colors[c];
            rasterize_triangle(&raster_triangle, color, false);
        }
        
        c++;
    }

    temp_set_size(temp_old_size);
}




// @todo figure out what is z exactly in raster space??


// z layers for rect
// draw rect outline (should be done with lines)
// clamp rect to viewport..
// constant access to frame and z buffers through x,y (i,j), and index
// rasterize axis-aligned quad
// 2d rotation? transpose/translation .?
// color functionality..

// draw line, anti-aliasing, clipping, projection, z-buffering

// slope between vectors




#include "test2.cpp"

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

extern "C" Engine_frame_result update_and_render(Platform_input_pass input)
{
    process_input(input);

    Engine_frame_result result;
    result.show_cursor = true;
    
    Engine_state* engine_state = ENGINE_STATE;
    Vector2 cursor_difference;
    cursor_difference.x = -(CURSOR_X - FRAMEBUFFER_WIDTH/2);
    cursor_difference.y = CURSOR_Y - FRAMEBUFFER_HEIGHT/2;

    Camera* camera = MAIN_CAMERA;
    r32 x_angle = cursor_difference.y / (2 * KB);
    r32 y_angle = cursor_difference.x / (2 * KB);

    // rotating around world Y because we don't wanna move diagonally when moving mouse left/right, but always around world Y
    // this is not the case when rotating around X, we wanna rotate around local X
    Quaternion rot_around_y = quaternion_from_axis(vec_up(), y_angle);
    Quaternion rot_around_x = quaternion_from_axis(quaternion_rot_vector(vec_right(), camera->orientation), x_angle);
    
    camera->orientation = quaternion_chain(quaternion_chain(camera->orientation, rot_around_x), rot_around_y);

    if (!engine_state->normalization_counter)
    {
        camera->orientation = quaternion_normalize(camera->orientation);
    }

    // @todo fix moving faster diagonally if you care...
    Vector3 camera_movement = vec_zero3();
    if (get_key(KEY_W))
    {
        camera_movement.z -= 2.0f;
    }
    if (get_key(KEY_S))
    {
        camera_movement.z += 2.0f;
    }
    if (get_key(KEY_A))
    {
        camera_movement.x -= 2.0f;
    }
    if (get_key(KEY_D))
    {
        camera_movement.x += 2.0f;
    }
    if (get_key(KEY_Q)) // @todo y movement should be in world y, not based on camera orientation
    {
        camera_movement.y -= 2.0f;
    }
    if (get_key(KEY_E))
    {
        camera_movement.y += 2.0f;
    }

    camera_movement = quaternion_rot_vector(camera_movement, camera->orientation);
    camera->position = vec_add(camera->position, camera_movement);

    
    
    
    Framebuffer framebuffer = engine_state->framebuffer;
    r32* zbuffer = engine_state->zbuffer;
    s32 bytpp = BYTPP;
    s32 pitch = framebuffer_pitch(framebuffer.height, bytpp);

    fill_background();
    
    Matrix4* view = view_tmatrix_for_camera();
    Matrix4* proj = perspective_tmatrix_for_camera();
    
    model_matrix_test(view, proj);
    
    zbuffer_reset(zbuffer, framebuffer.width, framebuffer.height);
    temp_reset();
    engine_state->normalization_counter++;

    return result;
}

