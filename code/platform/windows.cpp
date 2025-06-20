#if USE_DLL
#include <Windows.h>
#endif
#include <Memoryapi.h>
#include <intrin.h>
#include <malloc.h> // @todo do I need this?

#if USE_DLL

HMODULE load_dll()
{
    CopyFile(ACTUAL_DLL, COPIED_DLL, FALSE);
    HMODULE dll = LoadLibrary(COPIED_DLL);
    
    if (dll)
    {
        engine_api.platform_init_engine = (Platform_init_engine) GetProcAddress(dll, "platform_init_engine");
        engine_api.platform_init_memory_base = (Platform_init_memory_base) GetProcAddress(dll, "platform_init_memory_base");
        engine_api.update_and_render = (Update_and_render) GetProcAddress(dll, "update_and_render");
        
        if (!engine_api.platform_init_engine) engine_api.platform_init_engine = platform_init_engine_stub;
        if (!engine_api.platform_init_memory_base) engine_api.platform_init_memory_base = platform_init_memory_base_stub;
        if (!engine_api.update_and_render) engine_api.update_and_render = update_and_render_stub;
    }
    else
    {
        engine_api.platform_init_engine = platform_init_engine_stub;
        engine_api.platform_init_memory_base = platform_init_memory_base_stub;
        engine_api.update_and_render = update_and_render_stub;
    }

    return dll;
}

void unload_dll(HMODULE dll)
{
    FreeLibrary(dll);
    //DeleteFile(COPIED_DLL);
    engine_api.platform_init_engine = platform_init_engine_stub;
    engine_api.platform_init_memory_base = platform_init_memory_base_stub;
    engine_api.update_and_render = update_and_render_stub;
}
#endif


global_variable u8 keymap[256];
global_variable Input* input;

// because we're not sure how other platform works we use
// this to map between VK codes and engine's KEYCODE enum
// @todo init the rest to 0?
void init_keymap()
{
    keymap[VK_LEFT] = 1;
    keymap[VK_UP] = 2;
    keymap[VK_RIGHT] = 3;
    keymap[VK_DOWN] = 4;
    keymap['W'] = 5;
    keymap['S'] = 6;
    keymap['A'] = 7;
    keymap['D'] = 8;
    keymap['Q'] = 9;
    keymap['E'] = 10;
    keymap['I'] = 11;
    keymap['K'] = 12;
    keymap['J'] = 13;
    keymap['L'] = 14;
    keymap['U'] = 15;
    keymap['O'] = 16;
}

// @todo maybe we should never assert with these functions, but it's
// easy to remove that then, and for those that have void return type
// just change to b32 (only write_file has this; for everything else
// you mostly would just have to remove ASSERT(false) stuff everywhere)

// @todo maybe don't inline these??

inline b32 open_existing_file_for_reading(const u8* filename, HANDLE* handle)
{
    HANDLE h = CreateFile((LPCSTR)filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (h == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) return false;
        
        ASSERT(false);
        return false;
    }
    *handle = h;
    return true;
}

inline b32 open_existing_file_for_writing(const u8* filename, HANDLE* handle)
{
    HANDLE h = CreateFile((LPCSTR)filename, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (h == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) return false;
        
        ASSERT(false);
        return false;
    }
    *handle = h;
    return true;
}

inline void create_or_open_file_for_writing(const u8* filename, HANDLE* handle)
{
    HANDLE h = CreateFile((LPCSTR)filename, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    // this will basically happen with errors that I don't want to process at all
    ASSERT(h != INVALID_HANDLE_VALUE);
    *handle = h;
}

inline b32 get_file_size(const u8* filename, u32* size)
{
    HANDLE h;
    if (!open_existing_file_for_reading(filename, &h)) return false;

    DWORD file_size = GetFileSize(h, 0);
    if (file_size == INVALID_FILE_SIZE && GetLastError() != NO_ERROR)
    {
        CloseHandle(h);
        return false;
    }

    CloseHandle(h);
    *size = file_size;
    return true;
}

inline b32 read_entire_file(const u8* filename, void* buffer, u32 buffer_size)
{
    HANDLE h;
    if (!open_existing_file_for_reading(filename, &h)) return false;

    DWORD bytes_read;
    if (!ReadFile(h, buffer, buffer_size, &bytes_read, 0) || bytes_read != buffer_size)
    {
        CloseHandle(h);
        ASSERT(false);
        return false;
    }

    CloseHandle(h);
    return true;
}

inline void write_entire_file(const u8* filename, void* buffer, u32 buffer_size)
{
    HANDLE h;
    create_or_open_file_for_writing(filename, &h);
    
    u32 bytes_written;
    if ((!WriteFile(h, buffer, buffer_size, (LPDWORD)&bytes_written, 0)) || (bytes_written != buffer_size))
    {
        CloseHandle(h);
        ASSERT(false);
    }

    CloseHandle(h);
}

inline b32 write_entire_existing_file(const u8* filename, void* buffer, u32 buffer_size)
{
    HANDLE h;
    if (!open_existing_file_for_writing(filename, &h)) return false;

    u32 bytes_written;
    if ((!WriteFile(h, buffer, buffer_size, (LPDWORD)&bytes_written, 0)) || (bytes_written != buffer_size))
    {
        CloseHandle(h);
        ASSERT(false);
    }

    CloseHandle(h);
    return true;
}

inline b32 copy_file(const u8* src, const u8* dest)
{
    if (!CopyFile((LPCSTR)src, (LPCSTR)dest, true))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) return false;

        ASSERT(false);
        return false;        
    }
    return true;
}

inline b32 copy_and_maybe_overwrite_file(const u8* src, const u8* dest)
{
    if (!CopyFile((LPCSTR)src, (LPCSTR)dest, false))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) return false;

        ASSERT(false);
        return false;
    }
    return true;
}

inline b32 move_file(const u8* src, const u8* dest)
{
    if (!MoveFileEx((LPCSTR)src, (LPCSTR)dest, MOVEFILE_COPY_ALLOWED))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) return false;

        ASSERT(false);
        return false;
    }
    return true;
}

inline b32 move_and_maybe_overwrite_file(const u8* src, const u8* dest)
{
    if (!MoveFileEx((LPCSTR)src, (LPCSTR)dest, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) return false;

        ASSERT(false);
        return false;
    }
    return true;
}

inline b32 delete_file(const u8* filename)
{
    if (!DeleteFile((LPCSTR)filename))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) return false;

        ASSERT(false);
        return false;
    }
    return true;
}

inline b32 create_directory(const u8* dirname)
{
    if (!CreateDirectory((LPCSTR)dirname, 0))
    {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) return false;
        
        ASSERT(false);
        return false;
    }
    return true;
}

// @doc only deletes empty directories
inline b32 delete_directory(const u8* dirname)
{
    if (!RemoveDirectory((LPCSTR)dirname))
    {
        DWORD error = GetLastError();
        if (error == ERROR_DIR_NOT_EMPTY) return false;

        ASSERT(false);
        return false;
    }
    return true;
}

inline b32 directory_exists(const u8* dirname)
{
    DWORD att = GetFileAttributes((LPCSTR)dirname);
    if (att == INVALID_FILE_ATTRIBUTES) return false;
    return (att & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

inline r64 read_time_counter()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    // in milliseconds (ms)
    return count.QuadPart * 1000 / (r64) freq.QuadPart;
}


#if MAKE_FONT_BMP
void make_font_bmp()
{
    // @todo obviously we want to use .png eventually, but we need 3rd party software for that or
    // we need to use some very garbage windows api (WIC)
    
    // this function should be called once and then removed from the program, the only purpose of it
    // is to create a .bmp texture file with font glyphs once so we can load it into RAM at runtime later;
    // so this really should be a separate program but oh well...

    // .bmp can only record rgb, not a, so what you do is you set background transparent so it will
    // stay black (0, 0, 0), then set the color of text to white (255, 255, 255), so the inside of
    // a glyph will be white and on edges it will be gray because of anti-aliasing; then when you
    // are reading the file in order to load it into RAM, you create a 4th slot for alpha, you treat
    // all rgb=black as transparent (so alpha=0), you then convert to grayscale (or just use any of
    // the r/g/b since they will all be the same) and set that as the alpha, and then you set the
    // rgb to be white, then you can tint this to change the color of text basically, and you
    // can alpha blend text onto anything this way (I mean you don't need to test whether it's black
    // when you convert to grayscale and set that as the alpha, if it's black then alpha=0 automatically anyway)
    
    // this also means BITMAPINFO can have 24 bits per pixel because we're not using the alpha, but it's questionable
    // how compatible 24 bits per pixel is with winapi...
    
    // glyph count is 95, because ASCII printable characters are from 32 to 126 which is 95 total
    
    // when you start using gpu to do texture filtering, in order for it to sample correctly you should
    // add 1-4 pixels of padding around glyphs

    // we will do monospaced so to get height you use GetTextMetrics, to get width you need to find the max width
    // of each glyph with GetCharABCWidths by looping over all glyphs and calling this (possible with GetTextExtentPoint32A also.....)

    // if font is not monospaced we can center the glyph horizontally, by finding it's actualy width with GetCharABCWidths,
    // this way it ought to look a bit better
    
    // to load a font you use CreateFont, to create an actual bitmap you use CreateDIBSection (which needs a BITMAPINFO), to
    // draw a glyph into the bitmap you use TextOut/ExtTextOut, to set color of background or text you use stuff like
    // SetBkMode, SetBkColor, SetTextColor
    
    // you need to write BITMAPFILEHEADER first (14 B), BITMAPINFOHEADER (40 B) first into the .bmp, then the actual bitmap
    // from CreateDIBSection; when reading .bmp you take this into account

    // since we do a monospaced font, we don't need to write any other metadata, we just assume that the glyph position corresponding to
    // a char is dependant on the ASCII code of that char and that is how you can find the glyph for a char; the width, height, padding
    // and other stuff like that you remember elsewhere ???

    // render s32o bitmap at higher resolution like 64x64 or 128x128, and then scale down to lower resolution, but how to map this??
    // in CreateFont the first parameter specifies the height, you don't ensure width except by using a monospaced font and measuring max
    // width
    

    s32 first_ascii =  32;
    s32 last_ascii = 126;
    s32 num_glyphs = last_ascii - first_ascii;
    s32 glyph_padding = 4;
    s32 font_height = 64;

    HDC dc = CreateCompatibleDC(0);

    HFONT font = CreateFont(-font_height, 0, 0, 0,
                            FW_NORMAL, FALSE, FALSE, FALSE,
                            ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                            DEFAULT_PITCH, FONT_NAME);
    SelectObject(dc, font);

    // Get max width from all characters
    s32 glyph_width = 0;
    for (s32 c = first_ascii; c <= last_ascii; c++)
    {
        ABC abc;
        GetCharABCWidths(dc, c, c, &abc);
        s32 width = abc.abcA + abc.abcB + abc.abcC;
        glyph_width = Max(width, glyph_width);
    }

    TEXTMETRIC tm;
    GetTextMetrics(dc, &tm);
    s32 glyph_height = tm.tmHeight;

    s32 padded_w = glyph_width + glyph_padding * 2;
    s32 padded_h = glyph_height + glyph_padding * 2;

    // atlas(bmp data part) is a square in terms of glyph count in width and height
    s32 row_count = ceili(sqrtf((r32)num_glyphs));
    s32 atlas_width = row_count * padded_w;
    s32 atlas_height = row_count * padded_h;

    BITMAPINFO bmp = {0};
    bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp.bmiHeader.biWidth = atlas_width;
    bmp.bmiHeader.biHeight = -atlas_height;
    bmp.bmiHeader.biPlanes = 1;
    bmp.bmiHeader.biBitCount = 8 * 4; // BYTPP (I guess, you could use 24 instead but doesn't matter)
    bmp.bmiHeader.biCompression = BI_RGB;

    void* bmp_data = 0;
    HBITMAP bmp_handle = CreateDIBSection(dc, &bmp, DIB_RGB_COLORS, &bmp_data, 0, 0);
    SelectObject(dc, bmp_handle);

    // initializes bmp to black(0,0,0) everywhere
    PatBlt(dc, 0, 0, atlas_width, atlas_height, BLACKNESS);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255));

    // drawing into bmp
    for (s32 i = 0; i < num_glyphs; ++i)
    {
        s32 row = i / row_count;
        s32 col = i % row_count;
        s32 x = col * padded_w + glyph_padding;
        s32 y = row * padded_h + glyph_padding;

        u8 c = (u8)(first_ascii + i);
        SIZE size;
        GetTextExtentPoint32A(dc, (LPCSTR)&c, 1, &size); // use GetCharABCWidths instead??
        // this centers glyphs horizontally
        s32 offset_x = (glyph_width - size.cx) / 2;

        TextOutA(dc, x + offset_x, y, (LPCSTR)&c, 1);
    }

    // writing bmp file
    // @doc overwrites file if it already exists
    // @todo maybe do without all the preprocessor stuff if you can append strings??
    // @todo use provides one instead??
#define FONT_ATLAS_FILE_NAME FONTS_PATH FONT_NAME ".bmp"
    HANDLE bmp_file = CreateFileA(FONT_ATLAS_FILE_NAME, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
#undef FONT_ATLAS_FILE_NAME

    if (bmp_file != INVALID_HANDLE_VALUE)
    {
        s32 bmp_size = atlas_width * atlas_height * 4;

        BITMAPFILEHEADER bmp_header = {0};
        bmp_header.bfType = 0x4D42; // 'BM'         <- something better??
        bmp_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        bmp_header.bfSize = bmp_header.bfOffBits + bmp_size;

        DWORD written;
        WriteFile(bmp_file, &bmp_header, sizeof(BITMAPFILEHEADER), &written, 0);
        WriteFile(bmp_file, &bmp.bmiHeader, sizeof(BITMAPINFOHEADER), &written, 0);
        WriteFile(bmp_file, bmp_data, bmp_size, &written, 0);

        CloseHandle(bmp_file);
    }

    DeleteObject(bmp_handle);
    DeleteDC(dc);

    // I'm actually also writing the font metadata file so it has information about glyph padding, width and height,
    // what is implicit is ascii from first to last in that sequence, and that it's monospaced
#define FONT_DATA_FILE_NAME FONTS_PATH FONT_NAME ".font"
    HANDLE font_data_file = CreateFileA(FONT_DATA_FILE_NAME, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
#undef FONT_DATA_FILE_NAME
    
    if (font_data_file != INVALID_HANDLE_VALUE)
    {
        u8 font_data[5];
        font_data[0] = (u8)glyph_padding;
        *(u16*)(font_data + 1) = (u16)glyph_width;
        *(u16*)(font_data + 3) = (u16)glyph_height;

        DWORD written;
        WriteFile(font_data_file, font_data, sizeof(font_data), &written, 0);
        CloseHandle(font_data_file);
    }

}
#endif

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    // game don't do WM_SIZE :) ...
    
    switch(message)
    {
    case WM_ACTIVATEAPP:
    {
        OutputDebugString("WM_ACTIVATEAPP\n");
    } break;
    
    case WM_KEYUP:
    // case WM_SYSKEYUP:
    {
        KEYCODE key = (KEYCODE)keymap[(u8)wParam];
        if (bit_array_get(input->keys, key))
        {
            bit_array_set(input->keys_released, key);
        }
        bit_array_unset(input->keys, key);
    } break;
    case WM_KEYDOWN:
    // case WM_SYSKEYDOWN:
    {
        KEYCODE key = (KEYCODE)keymap[(u8)wParam];
        if (!bit_array_get(input->keys, key))
        {
            bit_array_set(input->keys_pressed, key);
        }
        bit_array_set(input->keys, key);
    } break;
    
    case WM_MOUSEMOVE:
    {
        // input->cursor_x = lParam & 0x0000FFFF;
        // input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        input->moved_mouse = true;
    } break;

    case WM_LBUTTONUP:
    {
        MKEYCODE mkey = MKEY_M1;
        if (get_flags(input->mkeys, MKEY_M1))
        {
            input->mkeys_released = set_flags(input->mkeys_released, mkey);
        }
        input->mkeys = unset_flags(input->mkeys, mkey);
    } break;
    case WM_LBUTTONDOWN:
    {
        MKEYCODE mkey = MKEY_M1;
        if (!get_flags(input->mkeys, mkey))
        {
            input->mkeys_pressed = set_flags(input->mkeys_pressed, mkey);
        }
        input->mkeys = set_flags(input->mkeys, mkey);
    } break;
    
    case WM_RBUTTONUP:
    {
        MKEYCODE mkey = MKEY_M2;
        if (get_flags(input->mkeys, mkey))
        {
            input->mkeys_released = set_flags(input->mkeys_released, mkey);
        }
        input->mkeys = unset_flags(input->mkeys, mkey);
    } break;
    case WM_RBUTTONDOWN:
    {
        MKEYCODE mkey = MKEY_M2;
        if (!get_flags(input->mkeys, mkey))
        {
            input->mkeys_pressed = set_flags(input->mkeys_pressed, mkey);
        }
        input->mkeys = set_flags(input->mkeys, mkey);
    } break;
    
    case WM_CLOSE:
    {
        PostQuitMessage(wParam);
    } break;

    case WM_DESTROY:
    {
        PostQuitMessage(wParam);
    } break;

    case WM_PAINT:
    {
        // you need this, or otherwise the window will be blank
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(window, &ps);
        EndPaint(window, &ps);
    } break;
            
    default:
    {
        result = DefWindowProc(window, message, wParam, lParam);
    }            
    }
    
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // @todo failure points??
    // @todo multimonitor support
    
    // window priority?
    
    timeBeginPeriod(1);
    init_keymap();

#if MAKE_FONT_BMP
    make_font_bmp();
#endif

#if USE_DLL
    HMODULE dll = load_dll();
    WIN32_FIND_DATA find_data;
    HANDLE dll_filehandle = FindFirstFile((LPCSTR)ACTUAL_DLL, &find_data);
    FindClose(dll_filehandle);
    FILETIME dll_filetime_prev = find_data.ftLastWriteTime;
    FILETIME dll_filetime_curr;
#endif

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    // VirtualAlloc aligns reserves and commits in chunks of this
    u64 allocation_step = (u64) Max(system_info.dwPageSize, system_info.dwAllocationGranularity);
    
    u64 total_program_memory = align_up(INITIAL_COMMIT_SIZE_BY_PLATFORM, allocation_step);
    void* base_ptr = VirtualAlloc(0, total_program_memory, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    PLATFORM_INIT_MEMORY_BASE((Globals*) base_ptr);
#if USE_DLL // because the platform has it's own globals variable
    globals = (Globals*) base_ptr;
#endif
    
    PLATFORM_API.total_program_memory = total_program_memory;
    PLATFORM_API.allocation_step = allocation_step;
#if USE_DLL
    PLATFORM_API.get_file_size = get_file_size;
    PLATFORM_API.read_entire_file = read_entire_file;
    PLATFORM_API.write_entire_file = write_entire_file;
    PLATFORM_API.copy_file = copy_file;
    PLATFORM_API.copy_and_maybe_overwrite_file = copy_and_maybe_overwrite_file;
    PLATFORM_API.move_file = move_file;
    PLATFORM_API.move_and_maybe_overwrite_file = move_and_maybe_overwrite_file;
    PLATFORM_API.delete_file = delete_file;
    PLATFORM_API.create_directory = create_directory;
    PLATFORM_API.delete_directory = delete_directory;
    PLATFORM_API.directory_exists = directory_exists;
    PLATFORM_API.read_time_counter = read_time_counter;
#endif

    // @todo do I also pass platform_api here so I initialize there instead?
    Platform_init_result init;
    PLATFORM_INIT_ENGINE(&init);
    input = init.input_address;

    WNDCLASS window_struct = {0};
    window_struct.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    window_struct.lpfnWndProc = window_procedure;
    window_struct.hInstance = hInstance;
    window_struct.lpszClassName = "my_window_struct";
    
    ATOM registered_window_struct = RegisterClass(&window_struct);

    HWND window = CreateWindow(window_struct.lpszClassName,
                               "VGENGINE",
                               // in this it will disable resizing of window by dragging it's border
                               WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                               // WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                               init.window_offs_x,
                               init.window_offs_y,
                               init.window_width,
                               init.window_height,
                               0,
                               0,
                               hInstance,
                               0);
    
    // default resolution is 1087x584, UL=(0,0)  ??
    BITMAPINFO window_buffer_info = {0};
    window_buffer_info.bmiHeader.biSize = sizeof(window_buffer_info.bmiHeader);
    window_buffer_info.bmiHeader.biPlanes = 1;
    window_buffer_info.bmiHeader.biBitCount = 4 * 8; // BYTPP is 4 @todo actually use the macro
    window_buffer_info.bmiHeader.biCompression = BI_RGB;
    window_buffer_info.bmiHeader.biWidth = init.window_width;
    window_buffer_info.bmiHeader.biHeight = -init.window_height;

    Engine_frame_result result;
    // some of these are kind of hardcoded initalized for the first frame
    result.show_cursor = false;
    result.resize = false;
    result.window_width = init.window_width;
    result.window_height = init.window_height;
    result.cursor_x = 0;
    result.cursor_y = 0;

    Platform_frame_pass pass;
    pass.running = true;

    b32 running = true;
    
    while (running)
    {
#if USE_DLL
        // maybe delete previous dll?
        dll_filehandle = FindFirstFile((LPCSTR)ACTUAL_DLL, &find_data);
        FindClose(dll_filehandle);
        dll_filetime_curr = find_data.ftLastWriteTime;
        if (dll_filetime_curr.dwLowDateTime > dll_filetime_prev.dwLowDateTime)
        {
            unload_dll(dll);

            dll = load_dll();
            dll_filetime_prev = dll_filetime_curr;
                    
            PLATFORM_INIT_MEMORY_BASE((Globals*) base_ptr);
        }
#endif         
        u64 begin_cycle_count = __rdtsc();
        u64 begin_time_count = read_time_counter();
        
        MSG message;
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                pass.running = false;
                running = false; // @todo make this better
                break;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // screen space is relative to the top-left corner of entire
        // screen, whereas client space is relative to the top-left
        // corner of the window

        // GetCursorPos and SetCursorPos require screen space, and
        // calculating the cursor position with window message is done
        // in client space

        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(window, &cursor);
        input->cursor_x = (r32) cursor.x;
        input->cursor_y = (r32) cursor.y;

        ShowCursor(result.show_cursor);
        if (GetForegroundWindow() == window) // if window is in focus then set cursor
        {
            POINT new_cursor = {(LONG)result.cursor_x, (LONG)result.cursor_y};
            ClientToScreen(window, &new_cursor);
            SetCursorPos(new_cursor.x, new_cursor.y);
        }
        
        UPDATE_AND_RENDER(&pass, &result);

        // @todo also handle window offset?
        // @todo how to allow use to drag window around?
        // @todo cursor/input thingy missaligned....?

        if (result.change_display)
        {
            if (result.fullscreen)
            {
                if (result.exclusive_fullscreen)
                {
                    LONG window_style = GetWindowLong(window, GWL_STYLE);

                    SetWindowLong(window, GWL_STYLE, window_style & (~WS_OVERLAPPEDWINDOW));

                    DEVMODE dm = {0};
                    dm.dmSize = sizeof(DEVMODE);
                    dm.dmPelsWidth = result.window_width;
                    dm.dmPelsHeight = result.window_height;
                    dm.dmBitsPerPel = 8 * 4; // BYTPP
                    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
                    ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

                    HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
                    MONITORINFO monitor_info = {0};
                    monitor_info.cbSize = sizeof(MONITORINFO);
                    GetMonitorInfo(monitor, &monitor_info);
                    RECT monitor_rect = monitor_info.rcMonitor;
                    
                    SetWindowPos(window,
                                 HWND_TOP,
                                 monitor_rect.left,
                                 monitor_rect.top,
                                 monitor_rect.right - monitor_rect.left,
                                 monitor_rect.bottom - monitor_rect.top,
                                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOZORDER);
                }
                else // borderless fullscreen
                {
                    LONG window_style = GetWindowLong(window, GWL_STYLE);

                    SetWindowLong(window, GWL_STYLE, window_style & (~WS_OVERLAPPEDWINDOW));

                    ChangeDisplaySettings(NULL, 0);
                    
                    HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
                    MONITORINFO monitor_info = {0};
                    monitor_info.cbSize = sizeof(MONITORINFO);
                    GetMonitorInfo(monitor, &monitor_info);
                    RECT monitor_rect = monitor_info.rcMonitor;
                    
                    SetWindowPos(window,
                                 HWND_TOP,
                                 monitor_rect.left,
                                 monitor_rect.top,
                                 monitor_rect.right - monitor_rect.left,
                                 monitor_rect.bottom - monitor_rect.top,
                                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOZORDER);
                }
            }
            else // windowed
            {
                SetWindowLong(window,
                              GWL_STYLE,
                              WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
                
                ChangeDisplaySettings(NULL, 0);
                
                ShowWindow(window, SW_RESTORE);

                SetWindowPos(window,
                             NULL,
                             result.window_offs_x,
                             result.window_offs_y,
                             result.window_width,
                             result.window_height,
                             SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOZORDER);
            }
        }
         
        // @todo probably figure out how to put this into the windowed/fullscreen code somehow...
        if (result.resize)
        {
            // make sure to always resize the buffer, but only
            // set window rect if windowed display mode...
            
            window_buffer_info.bmiHeader.biWidth = result.window_buffer_width;
            window_buffer_info.bmiHeader.biHeight = -result.window_buffer_height;

            // only makes a new rect that is desired
            RECT new_rect = {0, 0, result.window_width, result.window_height};
            AdjustWindowRect(&new_rect, WS_OVERLAPPEDWINDOW, FALSE);
            
            s32 window_width  = new_rect.right - new_rect.left;
            s32 window_height = new_rect.bottom - new_rect.top;
            
            // actually sets the new rect for the window
            SetWindowPos(window, 0,
                         0, 0,
                         window_width, window_height,
                         SWP_NOZORDER | SWP_NOMOVE); // retains current window offset
        }
        
        RECT window_rect;
        GetClientRect(window, &window_rect);
        s32 window_rect_width = window_rect.right - window_rect.left;
        s32 window_rect_height = window_rect.bottom - window_rect.top;
        
        // this is what actually draws to the window
        HDC dc = GetDC(window);
        StretchDIBits(dc,
                      0, 0,
                      window_rect_width,
                      window_rect_height,
                      0, 0,
                      window_buffer_info.bmiHeader.biWidth,
                      -window_buffer_info.bmiHeader.biHeight,
                      result.window_buffer_base,
                      &window_buffer_info,
                      DIB_RGB_COLORS,
                      SRCCOPY);
        ReleaseDC(window, dc);



        // @todo this should only happen in debug mode

        // @todo better caching and calculation and precision of these counters
        u64 end_cycle_count = __rdtsc();
        u64 end_time_count = read_time_counter();
        
        u64 cycles_per_frame = end_cycle_count - begin_cycle_count;
        r64 ms_per_frame = end_time_count - begin_time_count;

        u64 begin_sleep_time_count = read_time_counter();

        // capping frame rate to 60 fps
        // @todo this doesn't wait precisely for 60 fps, a little more, a little less, how to improve this and does it matter??
        if (result.cap_frame_rate)
        {
            r64 target_ms = 1000 / (r64)result.target_frame_rate;
            r64 ms_to_sleep = target_ms - ms_per_frame;
            if (ms_to_sleep > 1)
            {
                Sleep(ms_to_sleep);
            }
        }

        u64 end_sleep_time_count = read_time_counter();

        r64 slept_ms = end_sleep_time_count - begin_sleep_time_count;
        
        ms_per_frame += slept_ms;
            
        r64 fps = 1000 / ms_per_frame;
            
        char debug_str[256];
        sprintf(debug_str,"%f fps : %f ms : %I64d cycles\n", fps, ms_per_frame, cycles_per_frame );

        OutputDebugStringA(debug_str);
    }
    
    return 0;
}
 
