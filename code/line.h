union Line
{
    struct { r32 x0, y0, x1, y1; };
    struct { v2 P, Q; };
};

enum
{
    REGIONCODE_INSIDE = 0,
    REGIONCODE_LEFT   = (1 << 0),
    REGIONCODE_RIGHT  = (1 << 1),
    REGIONCODE_BOTTOM = (1 << 2),
    REGIONCODE_TOP    = (1 << 3)
};


u8 ObtainRegionFlags(v2 point);
b32 clip_line(Line* line);
void draw_wndline_zbuffered_aa(v2 P0, v2 P1, r32 z, u32 color);
void draw_wndline_aa(v2 P0, v2 P1, u32 color);
void draw_line(v2 origin, v2 point, u32 color);

