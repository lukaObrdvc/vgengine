#ifndef RECT_H
#define RECT_H

// @doc assumes y is up, x is right
union Rect
{
    struct {r32 min_x, min_y, max_x, max_y;};
    struct {Vector2 bot_left, top_right;};
};

union Recti
{
    struct {s32 min_x, min_y, max_x, max_y;};
    struct {Vector2i bot_left, top_right;};
};


// FUNCTIONS FOR r32

inline Rect rect_make(r32 min_x, r32 min_y, r32 max_x, r32 max_y)
{
    return {min_x, min_y, max_x, max_y};
}

inline r32 rect_width(Rect rect)
{
    return rect.max_x - rect.min_x;
}
inline r32 rect_height(Rect rect)
{
    return rect.max_y - rect.min_y;
}

inline Rect rect_from_bot_left(Vector2 bot_left, r32 width, r32 height)
{
    return {bot_left.x,
            bot_left.y,
            bot_left.x + width,
            bot_left.y + height};
}
inline Rect rect_from_bot_right(Vector2 bot_right, r32 width, r32 height)
{
    return {bot_right.x - width,
            bot_right.y,
            bot_right.x,
            bot_right.y + height};
}
inline Rect rect_from_top_left(Vector2 top_left, r32 width, r32 height)
{
    return {top_left.x,
            top_left.y - height,
            top_left.x + width,
            top_left.y};
}
inline Rect rect_from_top_right(Vector2 top_right, r32 width, r32 height)
{
    return {top_right.x - width,
            top_right.y - height,
            top_right.x,
            top_right.y};
}

// translating rect
// scaling?
// clamping

// z layers for rect
// draw rect outline (should be done with lines)
// clamp rect to viewport..
// draw rect

// @todo recti to rect, rect to recti
// @todo clip rect (like to viewport and stuff..)

// FUNCTIONS FOR s32

inline Recti rect_make(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
    return {min_x, min_y, max_x, max_y};
}

inline s32 rect_width(Recti rect)
{
    return rect.max_x - rect.min_x;
}
inline s32 rect_height(Recti rect)
{
    return rect.max_y - rect.min_y;
}




#endif
