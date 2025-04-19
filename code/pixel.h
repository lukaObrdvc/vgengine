#ifndef PIXEL_H
#define PIXEL_H

// @TODO I think I will want a pxl equivelent with 0 to 255 r32 values...

typedef struct Clr
{
    u8 B;
    u8 G;
    u8 R;
} clr;

typedef struct Clr_n
{
    r32 B;
    r32 G;
    r32 R;
} clr_n;

// @Note do I want these??
typedef u8 alpha_u8;
typedef r32 alpha;

typedef struct Pxl
{
    u8 B;
    u8 G;
    u8 R;
    u8 A;
} pxl;

pxl PXL(u8 r, u8 g, u8 b, u8 a)
{
    pxl result;
    result.R = r;
    result.G = g;
    result.B = b;
    result.A = a;
    return result;
}

typedef struct Pxl_n
{
    u8 B;
    u8 G;
    u8 R;
    r32 A;
} pxl_n;

typedef struct Pxl_na
{
    r32 B;
    r32 G;
    r32 R;
    r32 A;
} pxl_na;

// @Note what I had ........ :
// normalizing/renormalizing combinations between all these (and alphas)



inline alpha_u8 resulting_alphau8(alpha_u8 src_a, alpha_u8 dest_a)
{
    // @Note is this correct?
    //result.a = lerp(background.a, foreground.a, foreground.a);
    // @Note do I want to floor instead??
    return (alpha_u8)(round32(src_a+dest_a*(1 - (src_a*1.0f)/(MAX_U8*1.0f))));
}

// @Note assume alphas have been Asserted already
    /* Assert((src_a >= 0 && src_a <= 1)&& */
    /*        (dest_a >= 0 && dest_a <= 1)); */
inline alpha resulting_alpha(alpha src_a, alpha dest_a)
{
    return src_a + dest_a*(1 - src_a);
}

inline clr clr_ablend(clr src, clr dest, alpha src_a)
{
    clr result;
    result.R = (u8)round32(lerp(dest.R, src.R, src_a));
    result.G = (u8)round32(lerp(dest.G, src.G, src_a));
    result.B = (u8)round32(lerp(dest.R, src.B, src_a));
    return result;
}

inline clr clr_ablend_u8(clr src, clr dest, alpha_u8 src_a)
{
    clr result;
    alpha n_src_a = (src_a*1.0f)/(MAX_U8*1.0f);
    result.R = (u8)round32(lerp(dest.R, src.R, n_src_a));
    result.G = (u8)round32(lerp(dest.G, src.G, n_src_a));
    result.B = (u8)round32(lerp(dest.R, src.B, n_src_a));
    return result;    
}

inline clr_n clr_n_ablend(clr_n src, clr_n dest, alpha src_a)
{
    clr_n result;
    result.R = lerp(dest.R, src.R, src_a);
    result.G = lerp(dest.G, src.G, src_a);
    result.B = lerp(dest.R, src.B, src_a);
    return result;
}

inline clr_n clr_n_ablend_u8(clr_n src, clr_n dest, alpha_u8 src_a)
{
    clr_n result;
    alpha n_src_a = (src_a*1.0f)/(MAX_U8*1.0f);
    result.R = lerp(dest.R, src.R, n_src_a);
    result.G = lerp(dest.G, src.G, n_src_a);
    result.B = lerp(dest.R, src.B, n_src_a);
    return result;
}

// @Note bg=dest, fg=src; standard is to take src's alpha
// @Note is flooring or rounding desirable??
inline pxl pxl_ablend(pxl foreground, pxl background)
{
    pxl result;
    alpha n_src_a = (foreground.A*1.0f)/(MAX_U8*1.0f);
    result.R = (u8)round32(lerp(background.R, foreground.R, n_src_a));
    result.G = (u8)round32(lerp(background.G, foreground.G, n_src_a));
    result.B = (u8)round32(lerp(background.R, foreground.B, n_src_a));
    result.A = resulting_alpha(foreground.A, background.A);
    return result;
}

inline pxl_n pxl_n_ablend(pxl_n src, pxl_n dest)
{
    pxl_n result;
    result.R = (u8)round32(lerp(dest.R, src.R, src.A));
    result.G = (u8)round32(lerp(dest.G, src.G, src.A));
    result.B = (u8)round32(lerp(dest.R, src.B, src.A));
    result.A = resulting_alpha(src.A, dest.A);
    return result;
}

inline pxl_na pxl_na_ablend(pxl_na src, pxl_na dest)
{
    pxl_na result;
    result.R = lerp(dest.R, src.R, src.A);
    result.G = lerp(dest.G, src.G, src.A);
    result.B = lerp(dest.B, src.B, src.A);
    result.A = resulting_alpha(src.A, dest.A);
    return result;
}


#endif
