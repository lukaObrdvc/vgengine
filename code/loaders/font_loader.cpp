void load_font_permanently(String name)
{
    Engine_state* engine_state = ENGINE_STATE;
    ASSERT(engine_state->num_fonts < FONTS_CAPACITY);
    Font* font = &engine_state->fonts[engine_state->num_fonts];
    
    load_font_metadata(name, font);
    font->bmp = arena_push<u8>(PERMANENT_ARENA, font_bmp_bytesize(font));
    load_font_bmp(name, font);
    fix_alpha_for_font_bmp(font);
    premul_alpha_for_font_bmp(font);
    
    engine_state->num_fonts++;
}

inline Font* get_font(MYFONT f)
{
    ASSERT(f >= 0 && f < FONTS_CAPACITY);
    return &ENGINE_STATE->fonts[f];
}

