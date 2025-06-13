#if USE_DLL
#include <Windows.h>
#endif
#include <Memoryapi.h>
#include <DSound.h>

#include <intrin.h>
#include <malloc.h> // @TODO do I need this?


global_variable u8 keymap[256];

void init_keymap()
{
    // @todo init the rest to 0?
    
    keymap[0x25] = 1; // LEFT
    keymap[0x26] = 2; // UP
    keymap[0x27] = 3; // RIGHT
    keymap[0x28] = 4; // DOWN
    keymap[0x57] = 5; // W
    keymap[0x53] = 6; // S
    keymap[0x41] = 7; // A
    keymap[0x44] = 8; // D
    keymap[0x51] = 9; // Q
    keymap[0x45] = 10; // E
    keymap[0x49] = 11; // I
    keymap[0x4B] = 12; // K
    keymap[0x4A] = 13; // J
    keymap[0x4C] = 14; // L
    keymap[0x55] = 15; // U
    keymap[0x4F] = 16; // O

}

#if USE_DLL

enum MOUSECODE
{
    MOUSE_NONE = 0,
    MOUSE_MOVE = (1 << 0),
    MOUSE_M1   = (1 << 1),
    MOUSE_M2   = (1 << 2),
    MOUSE_M3   = (1 << 3)
};

global_variable b32 valid_dll = false;
#define ACTUAL_DLL BUILD_PATH "main.dll"
#define COPIED_DLL BUILD_PATH "main_copy.dll"

HMODULE load_game()
{
    CopyFile(ACTUAL_DLL, COPIED_DLL, FALSE);
    HMODULE dll = LoadLibrary(COPIED_DLL);
    
    if (dll)
    {
        valid_dll = true;
        engine_api.platform_init_engine = (Platform_init_engine) GetProcAddress(dll, "platform_init_engine");
        engine_api.platform_memory_base = (Platform_init_memory_base) GetProcAddress(dll, "platform_init_memory_base");
        engine_api.update_and_render = (Update_and_render) GetProcAddress(dll, "update_and_render");
        engine_api.process_input = (Process_input) GetProcAddress(dll, "process_input");
    }
    else
    {
        valid_dll = false;
        engine_api.platform_init_engine = platform_init_engine_stub;
        engine_api.platform_init_memory_base = platform_init_memory_base_stub;
        engine_api.update_and_render = update_and_render_stub;
        engine_api.process_input = process_input_stub;
    }

    if (!engine_api.platform_init_engine)
    {
        valid_dll = false;
        engine_api.platform_init_engine = platform_init_engine_stub;
    }
    if (!engine_api.platform_init_memory_base)
    {
        valid_dll = false;
        engine_api.platform_init_memory_base = platform_init_memory_base_stub;
    }
    if (!engine_api.update_and_render)
    {
        valid_dll = false;
        engine_api.update_and_render = update_and_render_stub;
    }
    if (!engine_api.process_input)
    {
        valid_dll = false;
        engine_api.process_input = process_input_stub;
    }

    return dll;
}

void unload_game(HMODULE dll)
{
    FreeLibrary(dll);
    //DeleteFile(COPIED_DLL);
    valid_dll = false;
    engine_api.platform_init_engine = platform_init_engine_stub;
    engine_api.platform_init_memory_base = platform_init_memory_base_stub;
    engine_api.update_and_render = update_and_render_stub;
    engine_api.process_input = process_input_stub;    
}
#endif

global_variable b32 running = true;
global_variable Input* input; // @todo do better for monolithic build

struct Window_rect_dims
{
    s32 width;
    s32 height;
};

inline Window_rect_dims get_window_rect_dims(HWND window)
{
    RECT window_rect;
    GetClientRect(window, &window_rect);
    Window_rect_dims rect;
    rect.width = window_rect.right - window_rect.left;
    rect.height = window_rect.bottom - window_rect.top;
    return rect;
}

HANDLE dbg_open_file(u8* filename)
{
    return CreateFile((LPCSTR)filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL, 0);
}

// @TODO: returning with error code ??
b32 read_file(u8* filename, void* buffer, u32* buffer_size)
{
    HANDLE filehandle = dbg_open_file(filename);
    if (filehandle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(filehandle);
        return false;
    }

    *buffer_size = (u32) GetFileSize(filehandle, 0);
    if (*buffer_size == INVALID_FILE_SIZE)
    {
        CloseHandle(filehandle);
        return false;
    }

    u32 bytes_read;
    ReadFile(filehandle, buffer, *buffer_size, (LPDWORD)&bytes_read, 0);
    if (bytes_read != *buffer_size)
    {
        CloseHandle(filehandle);
        return false;
    }

    CloseHandle(filehandle);
    return true;
}

b32 write_file(u8* filename, void* buffer, u32 buffer_size)
{
    HANDLE filehandle = dbg_open_file(filename);
    if (filehandle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(filehandle);
        return false;
    }

    u32 bytes_written;
    WriteFile(filehandle, buffer, buffer_size, (LPDWORD)&bytes_written, 0);
    if (bytes_written != buffer_size)
    {
        CloseHandle(filehandle);
        return false;
    }

    CloseHandle(filehandle);
    return true;
}

void stretch_and_draw_window_buffer(HDC dc, void* window_buffer_memory, BITMAPINFO* window_buffer_info, int window_width, int window_height)
{
    // @Note this is what actually blits to the scree/window
    // @Note what if we on purpose create a bitmap of different size in order to map it into the window??
    StretchDIBits(
        dc,
        0, 0,
        window_buffer_info->bmiHeader.biWidth,
        -window_buffer_info->bmiHeader.biHeight,
        0, 0,
        window_width,
        window_height,
        window_buffer_memory,
        window_buffer_info,
        DIB_RGB_COLORS,
        SRCCOPY
        );
}

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    
    switch(message)
    {
    case WM_ACTIVATEAPP:
    {
        OutputDebugString("WM_ACTIVATEAPP\n");
    } break;
    
    case WM_SIZE:
    {
        /* Window_rect_dims rect = get_Window_rect_dims(window); */
        /* realloc_window_bitmap_buffer(rect.width, rect.height); */
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
        input->cursor_x = lParam & 0x0000FFFF;
        input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        input->moved_mouse = true;
    } break;

    // @todo figure out if I need to make a cursor for these two (4...)
    case WM_LBUTTONUP:
    {
        input->cursor_x = lParam & 0x0000FFFF;
        input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        
        MKEYCODE mkey = MKEY_M1;
        if (get_flags(input->mkeys, MKEY_M1))
        {
            input->mkeys_released = set_flags(input->mkeys_released, mkey);
        }
        input->mkeys = unset_flags(input->mkeys, mkey);
    } break;
    case WM_LBUTTONDOWN:
    {
        input->cursor_x = lParam & 0x0000FFFF;
        input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        
        MKEYCODE mkey = MKEY_M1;
        if (!get_flags(input->mkeys, mkey))
        {
            input->mkeys_pressed = set_flags(input->mkeys_pressed, mkey);
        }
        input->mkeys = set_flags(input->mkeys, mkey);
    } break;
    
    case WM_RBUTTONUP:
    {
        input->cursor_x = lParam & 0x0000FFFF;
        input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        
        MKEYCODE mkey = MKEY_M2;
        if (get_flags(input->mkeys, mkey))
        {
            input->mkeys_released = set_flags(input->mkeys_released, mkey);
        }
        input->mkeys = unset_flags(input->mkeys, mkey);
    } break;
    case WM_RBUTTONDOWN:
    {
        input->cursor_x = lParam & 0x0000FFFF;
        input->cursor_y = (lParam & 0xFFFF0000) >> 16;
        
        MKEYCODE mkey = MKEY_M2;
        if (!get_flags(input->mkeys, mkey))
        {
            input->mkeys_pressed = set_flags(input->mkeys_pressed, mkey);
        }
        input->mkeys = set_flags(input->mkeys, mkey);
    } break;
    
    case WM_CLOSE:
    {
        OutputDebugString("WM_CLOSE\n");
        running = false;
        //PostQuitMessage(wParam);
    } break;

    case WM_DESTROY: // @Note when closing window with X message WM_CLOSE is sent not this, so what do with this...?
    {
        OutputDebugString("WM_DESTROY\n");
        running = false;
        //PostQuitMessage(wParam);
    } break;

    case WM_PAINT:
    {
        // @IMPORTANT you need this shit, or otherwise the window
        // will be blank, if you process the entire message queue,
        // if you don't then you can leave this blank....
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

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nShowCmd)
{
    // @todo failure points??
    
    timeBeginPeriod(1);
    init_keymap();

#if USE_DLL
    HMODULE dll = load_game();
    WIN32_FIND_DATA find_data;
    HANDLE dll_filehandle = FindFirstFile((LPCSTR)ACTUAL_DLL, &find_data);
    FindClose(dll_filehandle);
    FILETIME dll_filetime_prev = find_data.ftLastWriteTime;
    FILETIME dll_filetime_curr;
#endif

    s32 bytes_per_pixel = 4;

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    // VirtualAlloc aligns reserves and commits by this
    u64 allocation_step = (u64) Max(systemInfo.dwPageSize,
                                    systemInfo.dwAllocationGranularity);
    
    u64 total_program_memory = align_up(INITIAL_COMMIT_SIZE_BY_PLATFORM, allocation_step);
    void* base_ptr = VirtualAlloc(0, total_program_memory, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    PLATFORM_INIT_MEMORY_BASE((Globals*) base_ptr);
    
    PLATFORM_API.bytes_per_pixel = bytes_per_pixel;
    PLATFORM_API.total_program_memory = total_program_memory;
    PLATFORM_API.allocation_step = allocation_step;
#if USE_DLL
    PLATFORM_API.read_file = read_file;
    PLATFORM_API.write_file = write_file;
#endif

    Platform_init_out init;
    PLATFORM_INIT_ENGINE(&init);
    input = init.input_address;
    
    
    int window_offset_x = 50;
    int window_offset_y = 50;

    WNDCLASS registered_window_infostruct = {0};
    registered_window_infostruct.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    registered_window_infostruct.lpfnWndProc = window_procedure;
    registered_window_infostruct.hInstance = hInstance;
    registered_window_infostruct.lpszClassName = "CardcrafterWindowClass";
    
    ATOM registered_window = RegisterClass(&registered_window_infostruct);

    // @Note initial window dims (perhaps offset too), is not written by
    // platform, but by game and they can be inconsistent, think about
    // whose job it is to do that, and how to resize the window....
    // @TODO window resizing...
    
    HWND window = CreateWindow(registered_window_infostruct.lpszClassName,
                               "Cardcrafter",
                               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                               window_offset_x,
                               window_offset_y,
                               1280, // + 16,
                               720, // + 39,
                               0,
                               0,
                               hInstance,
                               0);
    
    // default resolution is 1087x584, UL=(0,0)
    BITMAPINFO window_buffer_info = {0};
    window_buffer_info.bmiHeader.biSize = sizeof(window_buffer_info.bmiHeader);
    window_buffer_info.bmiHeader.biPlanes = 1;
    window_buffer_info.bmiHeader.biBitCount = bytes_per_pixel*8;
    window_buffer_info.bmiHeader.biCompression = BI_RGB;

    POINT center = { (LONG)FRAMEBUFFER_WIDTH/2, (LONG)FRAMEBUFFER_HEIGHT/2 };
    ClientToScreen(window, &center);
    SetCursorPos(center.x, center.y);
    
    LARGE_INTEGER counter_frequency;
    QueryPerformanceFrequency(&counter_frequency);

    // b32 once = true;
    void* window_buffer_memory = (void*)(FRAMEBUFFER_BASE - FRAMEBUFFER_BYTESIZE + FRAMEBUFFER_PITCH);
    
    while (running)
    {
#if USE_DLL
        // maybe delete previous dll?
        // not perfectly reloading....
        dll_filehandle = FindFirstFile((LPCSTR)ACTUAL_DLL, &find_data);
        FindClose(dll_filehandle);
        dll_filetime_curr = find_data.ftLastWriteTime;
        if (dll_filetime_curr.dwLowDateTime > dll_filetime_prev.dwLowDateTime)
        {
            unload_game(dll);
            dll = load_game();
            dll_filetime_prev = dll_filetime_curr;
                    
            PLATFORM_INIT_MEMORY_BASE((Globals*) base_ptr);
        }
#endif         
        u64 begin_cycle_count = __rdtsc();

        LARGE_INTEGER begin_time_count;
        QueryPerformanceCounter(&begin_time_count);
            
        MSG message;
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
        }


        // can use get cursor pos through function call
        // instead of messages

        Engine_frame_result results = UPDATE_AND_RENDER();

        if (!results.show_cursor)
        {
            ShowCursor(false);
            if (GetForegroundWindow() == window)
            {
                SetCursorPos(center.x, center.y);
            }
        }
        else
        {
            ShowCursor(true);
        }

        window_buffer_info.bmiHeader.biWidth = FRAMEBUFFER_WIDTH;
        window_buffer_info.bmiHeader.biHeight = -FRAMEBUFFER_HEIGHT;

        // @TODO figure out if I need to have two different types of
        // resizing, one that resizes the buffer, and the other
        // that resizes the window rect, and then if not the same
        // it will stretch
        // @TODO resize window rect??? (AjdustWindowRect)
            
        HDC dc = GetDC(window);
        Window_rect_dims rect = get_window_rect_dims(window);
        stretch_and_draw_window_buffer(dc,
                                       window_buffer_memory,
                                       &window_buffer_info,
                                       rect.width,
                                       rect.height);
        ReleaseDC(window, dc);

        // @TODO better caching and calculation and precision of these counters
        u64 end_cycle_count = __rdtsc();

        u64 cycles_per_frame = end_cycle_count - begin_cycle_count;
            
        LARGE_INTEGER end_time_count;
        QueryPerformanceCounter(&end_time_count);

        r64 ms_per_frame = ((end_time_count.QuadPart - begin_time_count.QuadPart)*1000.0)/counter_frequency.QuadPart;

        LARGE_INTEGER begin_sleep_time_count;
        QueryPerformanceCounter(&begin_sleep_time_count);

        // @Note this doesn't wait precisely for 60fps, a little more, a little less, how to improve this and does it matter??
        if (ms_per_frame < 16.67)
        {
            Sleep((u32)(16.67-ms_per_frame));
        }
            
        LARGE_INTEGER end_sleep_time_count;
        QueryPerformanceCounter(&end_sleep_time_count);

        r64 slept_ms = ((end_sleep_time_count.QuadPart - begin_sleep_time_count.QuadPart)*1000.0)/counter_frequency.QuadPart;
        ms_per_frame+=slept_ms;
            
        r64 fps = 1000 / ms_per_frame;
            
        char debug_str[256];
        sprintf(debug_str,"%f fps : %f ms : %I64d cycles\n", fps, ms_per_frame, cycles_per_frame );

        // @Note turned off for now
        // OutputDebugStringA(debug_str);

        //Sleep(200);
    }

    return 0;
}
 
