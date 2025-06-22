#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#define NUM_ASCII_GLYPHS 95
#define FONT_METADATA_FILE_BYTESIZE 5
#define FONT_BMP_ROW_COUNT 10
#define FONT_BMP_COL_COUNT 10
// #define FONT_BMP_SLOT_COUNT 100
#define FONT_BMP_HEAD_BYTESIZE 54
#define FONTS_CAPACITY 1 // @todo rename to max fonts

// @doc implicitly monospaced
struct Font
{
    u8* bmp;
    s32 glyph_width;
    s32 glyph_height;
    s32 glyph_padding;
};


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

inline u32 font_bmp_bytesize(Font* font)
{
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

#endif
