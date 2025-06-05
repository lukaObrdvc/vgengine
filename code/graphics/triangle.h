#ifndef TRIANGLE_H
#define TRIANGLE_H

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


#endif
