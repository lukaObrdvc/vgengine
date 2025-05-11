u8 ObtainRegionFlags(v2 point)
{
    u8 result = REGIONCODE_INSIDE;

    if (point.x < 0)
        {
            result |= (u8)REGIONCODE_LEFT;
        }
    else if (point.x >= FRAME_BUFFER_WIDTH)
        {
            result |= (u8)REGIONCODE_RIGHT;
        }
    if (point.y < 0)
        {
            result |= (u8)REGIONCODE_BOTTOM;
        }
    else if (point.y >= FRAME_BUFFER_HEIGHT)
        {
            result |= (u8)REGIONCODE_TOP;
        }

    return result;
}

b32 clip_line(line* Line)
{
    b32 to_draw = false;

    u8 P_rflags = ObtainRegionFlags(Line->P);
    u8 Q_rflags = ObtainRegionFlags(Line->Q);

    while (true)
        {
            if (!(P_rflags | Q_rflags))   // both inside viewport, draw
                {
                    to_draw = true;
                    break;
                }
            else if (P_rflags & Q_rflags) // both outside viewport, don't draw
                {
                    break;
                }
            else                          // one outside, clip then iterate
                {
                    r32 ClippedX, ClippedY;
                    u8 rflags;
                    if (P_rflags > Q_rflags) // figure out which is outside
                        {
                            rflags = P_rflags;
                        }
                    else
                        {
                            rflags = Q_rflags;
                        }
                    
                    if (rflags & REGIONCODE_TOP) // clip
                        {
                            ClippedX = Line->x0 + (Line->x1 - Line->x0) * (FRAME_BUFFER_HEIGHT-1 - Line->y0) / (Line->y1 - Line->y0);
                            ClippedY = FRAME_BUFFER_HEIGHT-1;
                        }
                    else if (rflags & REGIONCODE_BOTTOM)
                        {
                            ClippedX = Line->x0 + (Line->x1 - Line->x0) * (0 - Line->y0) / (Line->y1 - Line->y0);
                            ClippedY = 0;
                        }
                    else if (rflags & REGIONCODE_RIGHT)
                        {
                            ClippedX = FRAME_BUFFER_WIDTH-1;
                            ClippedY = Line->y0 + (Line->y1 - Line->y0) * (FRAME_BUFFER_WIDTH-1 - Line->x0) / (Line->x1 - Line->x0);
                        }
                    else if (rflags & REGIONCODE_LEFT)
                        {
                            ClippedX = 0;
                            ClippedY = Line->y0 + (Line->y1 - Line->y0) * (0 - Line->x0) / (Line->x1 - Line->x0);
                        }

                    if (rflags == P_rflags) // update
                        {
                            Line->P.x = ClippedX;
                            Line->P.y = ClippedY;
                            P_rflags = ObtainRegionFlags(Line->P);
                        }
                    else
                        {
                            Line->Q.x = ClippedX;
                            Line->Q.y = ClippedY;
                            Q_rflags = ObtainRegionFlags(Line->Q);
                        }
                }
        }
    
    return to_draw;
}

void draw_line(v2 origin, v2 point, u32 color)
{
    // @TODO potentially say if lines on the outline of wndrect don't draw
    // or clamp lines in a different way

    // @TODO implement anti-aliasing
    
    u8* offset = FRAME_BUFFER + FRAME_BUFFER_PITCH*round_to_int(origin.y) + BYTPP*round_to_int(origin.x);
    u8* drawing_point = 0;
    
    r32 dX = point.x;
    r32 dY = point.y;
    s32 movebyX = 0;
    s32 movebyY = 0;
    r32 len = edist2(origin, point);
    r32 whenX = len / dX; // @Note divide by zero here??
    r32 whenY = len / dY;

    
    for (s32 i = 1; i <= Floor(len); i++)
        {
            movebyX = Floor(i/whenX);
            movebyY = Floor(i/whenY);
            drawing_point = offset + FRAME_BUFFER_PITCH*movebyY + BYTPP*movebyX;
                *((u32*)drawing_point) = color;
        }
}

void draw_wndline_aa(v2 P0, v2 P1, u32 color)
{
    b32 steep = abs(P1.y - P0.y) > abs(P1.x - P0.x);

    if (steep)
        {
            r32 tmp = P0.x;
            P0.x = P0.y;
            P0.y = tmp;

            tmp = P1.x;
            P1.x = P1.y;
            P1.y = tmp;
        }
    if (P0.x > P1.x)
        {
            r32 tmp = P0.x;
            P0.x = P1.x;
            P1.x = tmp;
            
            tmp = P0.y;
            P0.y = P1.y;
            P1.y = tmp;
        }

    r32 dx = P1.x - P0.x;
    r32 dy = P1.y - P0.y;
    r32 k = dy / dx;
    if (dx == 0.0f)
        {
            k = 1; // why exactly?
        }

    s32 xstart = Floor(P0.x); // round here?
    s32 xend = Floor(P1.x);
    r32 intersectY = P0.y;

    s32 xoffset;
    s32 yoffset;
    
    if (steep)
        {
            xoffset = FRAME_BUFFER_PITCH;
            yoffset = BYTPP;
        }
    else
        {
            xoffset = BYTPP;
            yoffset = FRAME_BUFFER_PITCH;
        }

    // clip xstart and xend here
    // also clip intersectY every iteration
    // the tricky part is probably figuring out all of the proper swaps...
    
    for (s32 x = xstart; x <= xend; x++)
        {
            // @IMPORTANT do we actually want to use 256 instead of 255, and in that case we can shift instead...
         
            
            u32* drawing_point1 = (u32*)(FRAME_BUFFER + floor_to_int(intersectY) * yoffset + x * xoffset);
            u32* drawing_point2 = (u32*)(FRAME_BUFFER + (floor_to_int(intersectY) - 1) * yoffset + x * xoffset);

            r32 alpha1 = 1 - decimal(intersectY);
            r32 alpha2 = decimal(intersectY);

            u32 color1 = *drawing_point1;
            u32 color2 = *drawing_point2;
                    
            r32 r1 = ((u8)((color1 & 0x00FF0000) >> 16)) / 255.0;
            r32 g1 = ((u8)((color1 & 0x0000FF00) >> 8)) / 255.0;
            r32 b1 = ((u8)(color1 & 0x000000FF)) / 255.0;

            r32 r2 = ((u8)((color2 & 0x00FF0000) >> 16)) / 255.0;
            r32 g2 = ((u8)((color2 & 0x0000FF00) >> 8)) / 255.0;
            r32 b2 = ((u8)(color2 & 0x000000FF)) / 255.0;

            r32 rc = ((u8)((color & 0x00FF0000) >> 16)) / 255.0;
            r32 gc = ((u8)((color & 0x0000FF00) >> 8)) / 255.0;
            r32 bc = ((u8)(color & 0x000000FF)) / 255.0;
                    
            r1 = lerp(rc, r1, alpha1);
            g1 = lerp(gc, g1, alpha1);
            b1 = lerp(bc, b1, alpha1);

            r2 = lerp(rc, r2, alpha2);
            g2 = lerp(gc, g2, alpha2);
            b2 = lerp(bc, b2, alpha2);

            u32 R1 = (u32)(r1 * 255);
            u32 G1 = (u32)(g1 * 255);
            u32 B1 = (u32)(b1 * 255);

            u32 R2 = (u32)(r2 * 255);
            u32 G2 = (u32)(g2 * 255);
            u32 B2 = (u32)(b2 * 255);
                    
            u32 final1 = ((255 << 24) |
                          (R1 << 16) |
                          (G1 << 8) |   
                          B1);        

            u32 final2 = ((255 << 24) |
                          (R2 << 16) |
                          (G2 << 8) |   
                          B2);        

            *drawing_point1 = final1;
            *drawing_point2 = final2;
                    
            intersectY += k;
        }
}

void draw_wndline_zbuffered_aa(v2 P0, v2 P1, r32 z, u32 color)
{
    b32 steep = abs(P1.y - P0.y) > abs(P1.x - P0.x);

    if (steep)
        {
            r32 tmp = P0.x;
            P0.x = P0.y;
            P0.y = tmp;

            tmp = P1.x;
            P1.x = P1.y;
            P1.y = tmp;
        }
    if (P0.x > P1.x)
        {
            r32 tmp = P0.x;
            P0.x = P1.x;
            P1.x = tmp;
            
            tmp = P0.y;
            P0.y = P1.y;
            P1.y = tmp;
        }

    r32 dx = P1.x - P0.x;
    r32 dy = P1.y - P0.y;
    r32 k = dy / dx;
    if (dx == 0.0f)
        {
            k = 1; // why exactly?
        }

    s32 xstart = Floor(P0.x); // round here?
    s32 xend = Floor(P1.x);
    r32 intersectY = P0.y;

    s32 xoffset;
    s32 yoffset;
    
    if (steep)
        {
            xoffset = FRAME_BUFFER_PITCH;
            yoffset = BYTPP;
        }
    else
        {
            xoffset = BYTPP;
            yoffset = FRAME_BUFFER_PITCH;
        }

    // clip xstart and xend here
    // also clip intersectY every iteration
    // the tricky part is probably figuring out all of the proper swaps...
    
    for (s32 x = xstart; x <= xend; x++)
        {
            r32* Z = (r32*)(Z_BUFFER + floor_to_int(intersectY)*yoffset + x*xoffset);
            if (z < *Z ||     // this stuff was other way when Z was other way, also >= on first condition
                z > ENGINESTATE->nearclip ||
                z < ENGINESTATE->farclip)
                {
                    continue;
                }
            else
                {
                    *Z = z;
                }
            
            // @IMPORTANT do we actually want to use 256 instead of 255, and in that case we can shift instead...
         
            
            u32* drawing_point1 = (u32*)(FRAME_BUFFER + floor_to_int(intersectY) * yoffset + x * xoffset);
            u32* drawing_point2 = (u32*)(FRAME_BUFFER + (floor_to_int(intersectY) - 1) * yoffset + x * xoffset);

            r32 alpha1 = 1 - decimal(intersectY);
            r32 alpha2 = decimal(intersectY);

            u32 color1 = *drawing_point1;
            u32 color2 = *drawing_point2;
                    
            r32 r1 = ((u8)((color1 & 0x00FF0000) >> 16)) / 255.0;
            r32 g1 = ((u8)((color1 & 0x0000FF00) >> 8)) / 255.0;
            r32 b1 = ((u8)(color1 & 0x000000FF)) / 255.0;

            r32 r2 = ((u8)((color2 & 0x00FF0000) >> 16)) / 255.0;
            r32 g2 = ((u8)((color2 & 0x0000FF00) >> 8)) / 255.0;
            r32 b2 = ((u8)(color2 & 0x000000FF)) / 255.0;

            r32 rc = ((u8)((color & 0x00FF0000) >> 16)) / 255.0;
            r32 gc = ((u8)((color & 0x0000FF00) >> 8)) / 255.0;
            r32 bc = ((u8)(color & 0x000000FF)) / 255.0;
                    
            r1 = lerp(rc, r1, alpha1);
            g1 = lerp(gc, g1, alpha1);
            b1 = lerp(bc, b1, alpha1);

            r2 = lerp(rc, r2, alpha2);
            g2 = lerp(gc, g2, alpha2);
            b2 = lerp(bc, b2, alpha2);

            u32 R1 = (u32)(r1 * 255);
            u32 G1 = (u32)(g1 * 255);
            u32 B1 = (u32)(b1 * 255);

            u32 R2 = (u32)(r2 * 255);
            u32 G2 = (u32)(g2 * 255);
            u32 B2 = (u32)(b2 * 255);
                    
            u32 final1 = ((255 << 24) |
                          (R1 << 16) |
                          (G1 << 8) |   
                          B1);        

            u32 final2 = ((255 << 24) |
                          (R2 << 16) |
                          (G2 << 8) |   
                          B2);        

            *drawing_point1 = final1;
            *drawing_point2 = final2;
                    
            intersectY += k;
        }
}

