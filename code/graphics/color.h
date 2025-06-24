#ifndef COLOR_H
#define COLOR_H


struct Color
{
    r32 r, g, b, a;
};

// @todo rename this to color_make, once you replace the old color_make that you currently have
inline Color color_make(r32 r, r32 g, r32 b, r32 a = 1.0f)
{
    return {r, g, b, a};
}

inline Color color_make_premultiplied(r32 r, r32 g, r32 b, r32 a = 1.0f)
{
    return {r * a, g * a, b * a, a};
}

inline Color color_premultiply(Color c)
{
    return {c.r * c.a, c.g * c.a, c.b * c.a, c.a};
}

inline Color color_unpremultiply(Color c)
{
    if (c.a == 0.0f) return color_make(0.0f, 0.0f, 0.0f, 0.0f);
    return {c.r / c.a, c.g / c.a, c.b / c.a, c.a};
}

inline Color color_lerp(const Color& c1, const Color& c2, r32 t)
{
    Color result;
    result.r = lerp(c1.r, c2.r, t);
    result.g = lerp(c1.g, c2.g, t);
    result.b = lerp(c1.b, c2.b, t);
    result.a = lerp(c1.a, c2.a, t);
    return result;
}

// @doc not commutative, blends src on top of dest
inline Color color_blend(const Color& src, const Color& dest)
{
    r32 resulting_alpha = src.a + dest.a * (1.0f - src.a);
    if (resulting_alpha == 0.0f) return color_make(0.0f, 0.0f, 0.0f, 0.0f);

    Color result;
    result.r = (src.r * src.a + dest.r * dest.a * (1.0f - src.a)) / resulting_alpha;
    result.g = (src.g * src.a + dest.g * dest.a * (1.0f - src.a)) / resulting_alpha;
    result.b = (src.b * src.a + dest.b * dest.a * (1.0f - src.a)) / resulting_alpha;
    result.a = resulting_alpha;
    return result;
}

// @doc not commutative, blends src on top of dest
// @todo does dest require to be premultiplied?
inline Color color_blend_premultiplied(const Color& src, const Color& dest)
{
    Color result;
    result.r = src.r + dest.r * (1.0f - src.a);
    result.g = src.g + dest.g * (1.0f - src.a);
    result.b = src.b + dest.b * (1.0f - src.a);
    result.a = src.a + dest.a * (1.0f - src.a);
    return result;
}

inline Color color_clamp(Color c)
{
    Color result;
    result.r = clamp(c.r, 0.0f, 1.0f);
    result.g = clamp(c.g, 0.0f, 1.0f);
    result.b = clamp(c.b, 0.0f, 1.0f);
    result.a = clamp(c.a, 0.0f, 1.0f);
    return result;
}

// @doc for little endian
inline u32 color_to_u32(Color color)
{
    color = color_clamp(color);
    
    u32 r = (u32)Floor(color.r * 255.0f);
    u32 g = (u32)Floor(color.g * 255.0f);
    u32 b = (u32)Floor(color.b * 255.0f);
    u32 a = (u32)Floor(color.a * 255.0f);
    
    u32 result;

    result = ((a << 24) | (r << 16) | (g << 8) | b);
    
    return result;
}

// @doc for little endian
inline Color u32_to_color(u32 color)
{
    Color result;
    
    u32 a = (color >> 24) & 0xFF;
    u32 r = (color >> 16) & 0xFF;
    u32 g = (color >> 8) & 0xFF;
    u32 b = color & 0xFF;

    result.r = r / 255.0f;
    result.g = g / 255.0f;
    result.b = b / 255.0f;
    result.a = a / 255.0f;
    
    return result;
}

// @doc if you want alpha of tint color to be present, then premultiply it
inline Color color_tint(Color c, Color tint)
{
    c.r *= tint.r;
    c.g *= tint.g;
    c.b *= tint.b;
    return color_clamp(c);
}

inline Color color_tint_lerp(Color c, Color tint, r32 t)
{
    return color_lerp(c, color_tint(c, tint), t);
}

inline Color color_brightness(const Color& c, r32 factor)
{
    Color result;
    result.r = c.r * factor;
    result.g = c.g * factor;
    result.b = c.b * factor;
    result.a = c.a;
    return result;
}

// @todo color theory stuff: hsv/hsb, luminance, grayscale, rgb<->hsv/b conversion
// @todo gamma correction?
// @todo make a pallete of a bunch of colors with color_white, color_black...

#endif
