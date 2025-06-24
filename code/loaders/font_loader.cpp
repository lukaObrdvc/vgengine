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

