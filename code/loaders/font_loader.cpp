void load_font_permanently(String name, MYFONT font_enum)
{
    Engine_state* engine_state = ENGINE_STATE;
    ASSERT(font_enum >= 0 && font_enum < NUM_FONTS);
    Font* font = &engine_state->fonts[font_enum];
    
    load_font_metadata(name, font);
    font->bmp = arena_push<u8>(PERMANENT_ARENA, font_bmp_bytesize(font));
    load_font_bmp(name, font);
    fix_alpha_for_font_bmp(font);
    premul_alpha_for_font_bmp(font);
}

inline Font* get_font(MYFONT f)
{
    ASSERT(f >= 0 && f < NUM_FONTS);
    return &ENGINE_STATE->fonts[f];
}


// because it's monospaced or something, some letters are not
// really looking like they're centered or something else is going on


void draw_string(const String& word,
                 Font* font,
                 const Vector2& offset,
                 const Vector2& scale,
                 const Color& tint,
                 const Rect& rect = {0.0f, 0.0f, (r32)FRAMEBUFFER_WIDTH, (r32)FRAMEBUFFER_HEIGHT})
{
    // @todo obviously there is aliasing because you're flooring the
    // pixel coordinates and offsets (and rect to clip on?)
    
    for (s32 c = 0; c < word.length; c++)
    {
        u8* glyph = get_glyph_bmp(font, word.base[c]);
    
        // this is texel space
        s32 glyph_w = font->glyph_width + 2 * font->glyph_padding;
        s32 glyph_h = font->glyph_height + 2 * font->glyph_padding;
        s32 stride = FONT_BMP_ROW_COUNT * glyph_w * BYTPP;
        
        // this is pixel space
        s32 scaled_w = floori(glyph_w * scale.x);
        s32 scaled_h = floori(glyph_h * scale.y);
        Vector2 letter_offset = vec_make(c * font->glyph_width * scale.x, 0.0f);
        letter_offset = vec_add(letter_offset, offset);

        // drawing is done in pixel space
        for (s32 j = 0; j < scaled_h; j++)
        {
            for (s32 i = 0; i < scaled_w; i++)
            {
                s32 drawpoint_x = i + floori(letter_offset.x);
                s32 drawpoint_y = j + floori(letter_offset.y);
                
                // clipping
                if ((drawpoint_x >= floori(rect.min_x) && drawpoint_x < floori(rect.max_x)) &&
                    (drawpoint_y >= floori(rect.min_y) && drawpoint_y < floori(rect.max_y)))
                {
                    // you want to sample from a texture when scale != 1, because then num_texels != num_pixels,
                    // and then you have to do texture filtering so that texels map to pixels in a good-looking way
            
                    // texel coordinates (but not normalized in this case)
                    r32 u = i / scale.x;
                    r32 v = (scaled_h - 1 - j) / scale.y; // inverts glyph vertically
            
                    Color src = bilinear_sample_premultiplied(glyph, glyph_w, glyph_h, stride, u, v);
                    src = color_tint(src, tint);
                    // this is not premultiplied, but it doesn't matter because you always blend ONTO it so alpha is not used ever
                    Color dest = u32_to_color(*framebuffer_access(drawpoint_x, drawpoint_y));
                    Color blended = color_blend_premultiplied(src, dest);
                    *framebuffer_access(drawpoint_x, drawpoint_y) = color_to_u32(blended);
                }
            }
        }
    }
}

void draw_string_wrapped(const String& word,
                         Font* font,
                         const Vector2& offset,
                         const Vector2& scale,
                         const Color& tint,
                         const r32& line_spacing,
                         const Rect& rect = {0.0f, 0.0f, (r32)FRAMEBUFFER_WIDTH, (r32)FRAMEBUFFER_HEIGHT})
{
    for (s32 c = 0; c < word.length; c++)
    {
        u8* glyph = get_glyph_bmp(font, word.base[c]);
    
        // this is texel space
        s32 glyph_w = font->glyph_width + 2 * font->glyph_padding;
        s32 glyph_h = font->glyph_height + 2 * font->glyph_padding;
        s32 stride = FONT_BMP_ROW_COUNT * glyph_w * BYTPP;
        
        // this is pixel space
        s32 scaled_w = floori(glyph_w * scale.x);
        s32 scaled_h = floori(glyph_h * scale.y);
        Vector2 letter_offset = vec_make(c * font->glyph_width * scale.x, 0.0f);
        letter_offset = vec_add(letter_offset, offset);

        // drawing is done in pixel space
        for (s32 j = 0; j < scaled_h; j++)
        {
            for (s32 i = 0; i < scaled_w; i++)
            {
                s32 drawpoint_x = i + floori(letter_offset.x);
                s32 drawpoint_y = j + floori(letter_offset.y);
                
                // clipping
                if ((drawpoint_x >= floori(rect.min_x) && drawpoint_x < floori(rect.max_x)) &&
                    (drawpoint_y >= floori(rect.min_y) && drawpoint_y < floori(rect.max_y)))
                {
                    // you want to sample from a texture when scale != 1, because then num_texels != num_pixels,
                    // and then you have to do texture filtering so that texels map to pixels in a good-looking way
            
                    // texel coordinates (but not normalized in this case)
                    r32 u = i / scale.x;
                    r32 v = (scaled_h - 1 - j) / scale.y; // inverts glyph vertically
            
                    Color src = bilinear_sample_premultiplied(glyph, glyph_w, glyph_h, stride, u, v);
                    src = color_tint(src, tint);
                    // this is not premultiplied, but it doesn't matter because you always blend ONTO it so alpha is not used ever
                    Color dest = u32_to_color(*framebuffer_access(drawpoint_x, drawpoint_y));
                    Color blended = color_blend_premultiplied(src, dest);
                    *framebuffer_access(drawpoint_x, drawpoint_y) = color_to_u32(blended);
                }
            }
        }
    }   
}

