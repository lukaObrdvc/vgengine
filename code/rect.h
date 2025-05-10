#ifndef RECT_H
#define RECT_H

// @doc assumes y is up, x is right
union Rect
{
    struct {r32 minX, minY, maxX, maxY;};
    struct {Vec2f botLeft, topRight;};
};

union RectInt
{
    struct {s32 minX, minY, maxX, maxY;};
    struct {Vec2i botLeft, topRight;};
};


// FUNCTIONS FOR 332

inline Rect rect_make(r32 minX, r32 minY, r32 maxX, r32 maxY)
{
    return {minX, minY, maxX, maxY};
}

inline r32 rect_width(Rect rect)
{
    return rect.maxX - rect.minX;
}
inline r32 rect_height(Rect rect)
{
    return rect.maxY - rect.minY;
}

inline Rect rect_from_botLeft(Vec2f botLeft, r32 width, r32 height)
{
    return {botLeft.x,
            botLeft.y,
            botLeft.x + width,
            botLeft.y + height};
}
inline Rect rect_from_botRight(Vec2f botRight, r32 width, r32 height)
{
    return {botRight.x - width,
            botRight.y,
            botRight.x,
            botRight.y + height};
}
inline Rect rect_from_topLeft(Vec2f topLeft, r32 width, r32 height)
{
    return {topLeft.x,
            topLeft.y - height,
            topLeft.x + width,
            topLeft.y};
}
inline Rect rect_from_topRight(Vec2f topRight, r32 width, r32 height)
{
    return {topRight.x - width,
            topRight.y - height,
            topRight.x,
            topRight.y};
}

// translating rect
// scaling?
// clamping
// look at unity stuff, and think about some stuff yourself

// FUNCTIONS FOR s32

inline RectInt rect_make(s32 minX, s32 minY, s32 maxX, s32 maxY)
{
    return {minX, minY, maxX, maxY};
}

inline s32 rect_width(RectInt rect)
{
    return rect.maxX - rect.minX;
}
inline s32 rect_height(RectInt rect)
{
    return rect.maxY - rect.minY;
}




#endif
