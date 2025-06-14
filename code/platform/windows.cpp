#if USE_DLL
#include <Windows.h>
#endif
#include <Memoryapi.h>
#include <DSound.h>

#include <intrin.h>
#include <malloc.h> // @todo do I need this?

#if USE_DLL

global_variable b32 valid_dll = false;

HMODULE load_dll()
{
    CopyFile(ACTUAL_DLL, COPIED_DLL, FALSE);
    HMODULE dll = LoadLibrary(COPIED_DLL);
    
    if (dll)
    {
        valid_dll = true;
        engine_api.platform_init_engine = (Platform_init_engine) GetProcAddress(dll, "platform_init_engine");
        engine_api.platform_memory_base = (Platform_init_memory_base) GetProcAddress(dll, "platform_init_memory_base");
        engine_api.update_and_render = (Update_and_render) GetProcAddress(dll, "update_and_render");
    }
    else
    {
        valid_dll = false;
        engine_api.platform_init_engine = platform_init_engine_stub;
        engine_api.platform_init_memory_base = platform_init_memory_base_stub;
        engine_api.update_and_render = update_and_render_stub;
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

    return dll;
}

void unload_dll(HMODULE dll)
{
    FreeLibrary(dll);
    //DeleteFile(COPIED_DLL);
    valid_dll = false;
    engine_api.platform_init_engine = platform_init_engine_stub;
    engine_api.platform_init_memory_base = platform_init_memory_base_stub;
    engine_api.update_and_render = update_and_render_stub;
}
#endif



// @todo do better for monolithic build
global_variable u8 keymap[256];
global_variable Input* input;




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

// PROVIDES

HANDLE dbg_open_file(u8* filename)
{
    return CreateFile((LPCSTR)filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL, 0);
}

// @todo returning with error code ??
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
        // ..?
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

    // @todo figure out if I need to make a cursor for these 4
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
    
    timeBeginPeriod(1);
    init_keymap();

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
    
    PLATFORM_API.total_program_memory = total_program_memory;
    PLATFORM_API.allocation_step = allocation_step;
#if USE_DLL
    PLATFORM_API.read_file = read_file;
    PLATFORM_API.write_file = write_file;
#endif

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
                               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
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

    // @cleanup counters
    LARGE_INTEGER counter_frequency;
    QueryPerformanceFrequency(&counter_frequency);

    Engine_frame_result result;
    // these 2 are kinda hardcoded to initialize like this only one frame
    result.show_cursor = false;
    result.recenter_cursor = true;
    result.window_buffer_width = init.window_width;
    result.window_buffer_height = init.window_height;

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

        LARGE_INTEGER begin_time_count;
        QueryPerformanceCounter(&begin_time_count);

        
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
        

        // can use get cursor pos through function call
        // instead of messages

        ShowCursor(result.show_cursor);
        if (result.recenter_cursor && (GetForegroundWindow() == window))
        {
            POINT center = {(LONG)result.window_buffer_width / 2,
                            (LONG)result.window_buffer_height / 2};
            ClientToScreen(window, &center);
            SetCursorPos(center.x, center.y);
        }
        
        UPDATE_AND_RENDER(&pass, &result);

        window_buffer_info.bmiHeader.biWidth = result.window_buffer_width;
        window_buffer_info.bmiHeader.biHeight = -result.window_buffer_height;

        // @todo figure out if I need to have two different types of
        // resizing, one that resizes the buffer, and the other
        // that resizes the window rect, and then if not the same
        // it will stretch
        // @todo resize window rect??? (AjdustWindowRect)

        RECT window_rect;
        GetClientRect(window, &window_rect);
        s32 window_rect_width = window_rect.right - window_rect.left;
        s32 window_rect_height = window_rect.bottom - window_rect.top;
        
        // this is what actually draws to the window
        HDC dc = GetDC(window);
        StretchDIBits(dc,
                      0, 0,
                      window_buffer_info.bmiHeader.biWidth,
                      -window_buffer_info.bmiHeader.biHeight,
                      0, 0,
                      window_rect_width,
                      window_rect_height,
                      result.window_buffer_base,
                      &window_buffer_info,
                      DIB_RGB_COLORS,
                      SRCCOPY);
        ReleaseDC(window, dc);



        // @todo this should only happen in debug mode

        // @todo better caching and calculation and precision of these counters
        u64 end_cycle_count = __rdtsc();

        u64 cycles_per_frame = end_cycle_count - begin_cycle_count;
            
        LARGE_INTEGER end_time_count;
        QueryPerformanceCounter(&end_time_count);

        r64 ms_per_frame = ((end_time_count.QuadPart - begin_time_count.QuadPart) * 1000.0) / counter_frequency.QuadPart;

        LARGE_INTEGER begin_sleep_time_count;
        QueryPerformanceCounter(&begin_sleep_time_count);

        // capping frame rate to 60 fps
        // @todo this doesn't wait precisely for 60 fps, a little more, a little less, how to improve this and does it matter??
        if (ms_per_frame < 16.67)
        {
            Sleep(16.67 - ms_per_frame);
        }
            
        LARGE_INTEGER end_sleep_time_count;
        QueryPerformanceCounter(&end_sleep_time_count);

        r64 slept_ms = ((end_sleep_time_count.QuadPart - begin_sleep_time_count.QuadPart) * 1000.0) / counter_frequency.QuadPart;
        
        ms_per_frame += slept_ms;
            
        r64 fps = 1000 / ms_per_frame;
            
        char debug_str[256];
        sprintf(debug_str,"%f fps : %f ms : %I64d cycles\n", fps, ms_per_frame, cycles_per_frame );

        OutputDebugStringA(debug_str);

        //Sleep(200);
    }

    return 0;
}
 
