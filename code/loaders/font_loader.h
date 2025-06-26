#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#define NUM_ASCII_GLYPHS 95
#define FIRST_ASCII 32
#define LAST_ASCII 126
#define FONT_METADATA_FILE_BYTESIZE 5
#define FONT_BMP_ROW_COUNT 10
#define FONT_BMP_COL_COUNT 10
#define FONT_BMP_HEAD_BYTESIZE 54
#define NUM_FONTS 4

// @doc implicitly monospaced
struct Font
{
    u8* bmp;
    s32 glyph_width;
    s32 glyph_height;
    s32 glyph_padding;
};

enum MYFONT
{
    MYFONT_NONE = -1,
    MYFONT_CONSOLAS8,
    MYFONT_CONSOLAS16,
    MYFONT_CONSOLAS32,
    MYFONT_CONSOLAS64
};

void load_font_permanently(String name);
inline Font* get_font(MYFONT f);
void draw_string(const String& word, Font* font, const Vector2& offset, const Vector2& scale, const Color& tint, const Rect& rect);
void draw_string_wrapped(const String& word, Font* font, const Vector2& offset, const Vector2& scale, const Color& tint, const r32& line_spacing, const Rect& rect);

// @doc we need glyph w,h,p first in order to allocate memory
// for the bitmap, so read the metadata file first
void load_font_metadata(String name, Font* font)
{
    Scratch* scratch = get_scratch();
    u8* buffer = scratch_push<u8>(scratch, FONT_METADATA_FILE_BYTESIZE);
    
    String filename = concat(name, str(".font"));
    String filepath = concat(str(FONT_PATH), filename);

    // @todo logging on fail
    READ_ENTIRE_FILE(cstr(filepath), buffer, FONT_METADATA_FILE_BYTESIZE);

    font->glyph_padding = (s32)buffer[0];
    font->glyph_width   = (s32)(*(u16*)(buffer + 1));
    font->glyph_height  = (s32)(*(u16*)(buffer + 3));
    
    release_scratch(scratch);
}

inline u32 font_bmp_bytesize(Font* font){
    return ((FONT_BMP_ROW_COUNT * (font->glyph_width  + 2 * font->glyph_padding)) *
            (FONT_BMP_COL_COUNT * (font->glyph_height + 2 * font->glyph_padding)) *
            BYTPP) + FONT_BMP_HEAD_BYTESIZE;
}

inline void load_font_bmp(String name, Font* font)
{
    String filename = concat(name, str(".bmp"));
    String filepath = concat(str(FONT_PATH), filename);
    
    // obviously you cannot skip reading the head part of the file because
    // it is simply written there like that all the time, so what you do instead
    // is you skip that part when reading the glyphs

    // @todo logging on fail
    READ_ENTIRE_FILE(cstr(filepath), font->bmp, font_bmp_bytesize(font));
}

inline void fix_alpha_for_font_bmp(Font* font)
{
    u32* bmp = (u32*)(font->bmp + FONT_BMP_HEAD_BYTESIZE);
    for (u32 i = 0; i < font_bmp_bytesize(font) - FONT_BMP_HEAD_BYTESIZE; i++)
    {
        Color c = u32_to_color(bmp[i]);
        c.a = c.r; // use grayscale instead?
        c.r = 1.0f;
        c.b = 1.0f;
        c.g = 1.0f;
        bmp[i] = color_to_u32(c);
    }
}

inline void premul_alpha_for_font_bmp(Font* font)
{
    u32* bmp = (u32*)(font->bmp + FONT_BMP_HEAD_BYTESIZE);
    for (u32 i = 0; i < font_bmp_bytesize(font) - FONT_BMP_HEAD_BYTESIZE; i++)
    {
        bmp[i] = color_to_u32(color_premultiply(u32_to_color(bmp[i])));
    }
}

inline s32 glyph_bytesize(Font* font)
{
    return (font->glyph_width + 2 * font->glyph_padding) * (font->glyph_height + 2 * font->glyph_padding) * BYTPP;
}

inline u8* get_glyph_bmp(Font* font, u8 c)
{
    ASSERT(c >= FIRST_ASCII && c <= LAST_ASCII);
    
    s32 glyph_index = c - FIRST_ASCII;
    
    s32 glyph_w = font->glyph_width + 2 * font->glyph_padding;
    s32 glyph_h = font->glyph_height + 2 * font->glyph_padding;

    s32 row = glyph_index / FONT_BMP_ROW_COUNT;
    s32 col = glyph_index % FONT_BMP_COL_COUNT;

    s32 stride = FONT_BMP_ROW_COUNT * glyph_w * BYTPP;

    return font->bmp + FONT_BMP_HEAD_BYTESIZE + row * glyph_h * stride + col * glyph_w * BYTPP;
}


#endif
