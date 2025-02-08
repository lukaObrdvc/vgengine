#ifndef RENDERER_H
#define RENDERER_H

// @Note length of a line is just edist(A,B)
typedef struct Line2
{
    v2 A;
    v2 B;
} line2;

typedef struct Line3
{
    v3 A;
    v3 B;
} line3;

// @Note A->B->C->D clockwise (is this a good idea?)
typedef struct Rect2
{
    v2 A;
    v2 B;
    v2 C;
    v2 D;
} rect2;

typedef struct Rect3
{
    v3 A;
    v3 B;
    v3 C;
    v3 D;
} rect3;


inline v2 rect_dims(rect2 rect)
{
    v2 result;
    result.x = edist2(rect.A, rect.B);
    result.y = edist2(rect.A, rect.D);
    return result;
}

inline v2 rect_dims3(rect3 rect)
{
    v2 result;
    result.x = edist3(rect.A, rect.B);
    result.y = edist3(rect.A, rect.D);
    return result;
}


#endif
