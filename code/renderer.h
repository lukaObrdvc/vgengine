#ifndef RENDERER_H
#define RENDERER_H

// @Note essentialy a rectangular portion of the window that can have a Z

// @Note you can associate a Z with this,
// so that you can layer multiple of these on each other
// and project them

// @Note must use y-is-up and x-is-right coordinate system,
// do not do preemptive abs on dims or figuring out min/max coords

// @Note also use z-is-towards-me (establishes right-handed coordsys)
typedef union
{
    struct
    {
        r32 left;    // x_min
        r32 bottom;  // y_min
        r32 right;   // x_max
        r32 top;     // y_max
    };
    struct
    {
        v2 ll;     // lower left  corner
        v2 ur;     // upper right corner
    };
} wndrect;

inline wndrect Wndrect(r32 l, r32 b, r32 r, r32 t)
{
    wndrect result;

    result.left = l;
    result.bottom = b;
    result.right = r;
    result.top = t;
    
    return result;
}

inline r32 wndrect_width(wndrect rect)
{
    r32 result;
    
    result = rect.right - rect.left;
    
    return result;
}

inline r32 wndrect_height(wndrect rect)
{
    r32 result;
    
    result = rect.top - rect.bottom;
    
    return result;
}

inline wndrect clamp_wndrect(wndrect rect)
{
    wndrect result = rect;

    clamp(result.left, 0, wnd_width);
    clamp(result.bottom, 0, wnd_height);
    clamp(result.right, 0, wnd_width);
    clamp(result.top, 0, wnd_height);
    
    /* if (rect.left < 0) */
    /*     { */
    /*         rect.width += rect.left; */
    /*         rect.left = 0; */
    /*     } */
    /* else if (rect.left > wnd_width) */
    /*     { */
    /*         rect.width = 0; */
    /*         rect.left = (r32)wnd_width; */
    /*     } */
    /* if (rect.top < 0) */
    /*     { */
    /*         rect.height += rect.top; */
    /*         rect.top = 0;             */
    /*     } */
    /* else if (rect.top > wnd_height) */
    /*     { */
    /*         rect.height = 0; */
    /*         rect.top = (r32)wnd_height; */
    /*     } */
    
    /* if (rect.left + rect.width > wnd_width) */
    /*     { */
    /*         rect.width = wnd_width - rect.left; */
    /*     } */
    /* if (rect.top + rect.height > wnd_height) */
    /*     { */
    /*         rect.height = wnd_height - rect.top; */
    /*     } */

    return result;
}

// @TODO probably put this into .c instead and don't inline

void draw_clamped_wndrect(wndrect rect, pxl color)
{
    // @TODO do I round?
    s32 offset = wnd_pitch*round32(rect.bottom) +
        round32(rect.left)*wnd_bytpp;
    s32 height = round32(wndrect_height(rect));
    s32 width = round32(wndrect_width(rect));
    
    for (s32 i = 0; i < height; i++)
        {
            pxl* row = (pxl*)(wnd_buffer + wnd_pitch*i + offset);
            for (s32 j = 0; j < width; j++)
                {
                    *row = color;
                    row++;
                }
        }
}

#endif
