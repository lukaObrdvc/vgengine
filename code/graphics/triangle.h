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

inline Triangle* ttriangle_4to3(Triangle4* t)
{
    Triangle* tri = temp_alloc(Triangle);
    tri->a = vec_4to3(t->a);
    tri->b = vec_4to3(t->b);
    tri->c = vec_4to3(t->c);
    return tri;
}


#endif
