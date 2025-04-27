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
    keymap[0x25] = 0;
    keymap[0x26] = 1; 
    keymap[0x27] = 2; 
    keymap[0x28] = 3; 

    keymap[0x57] = 4;   
    keymap[0x53] = 5; 
    keymap[0x41] = 6; 
    keymap[0x44] = 7; 
    keymap[0x51] = 8; 
    keymap[0x45] = 9; 

    keymap[0x49] = 10;   
    keymap[0x4B] = 11;
    keymap[0x4A] = 12;
    keymap[0x4C] = 13;
    keymap[0x55] = 14;
    keymap[0x4F] = 15;
}

#if USE_DLL

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
            engineAPI.platformInitMemoryBase = (fpPlatformInitMemoryBase) GetProcAddress(dll, "platform_init_memory_base");
            engineAPI.updateAndRender = (fpUpdateAndRender) GetProcAddress(dll, "update_and_render");
            engineAPI.processInput = (fpProcessInput) GetProcAddress(dll, "process_input");
        }
    else
        {
            valid_dll = false;
            engineAPI.platformInitMemoryBase = platform_init_memory_base_stub;
            engineAPI.updateAndRender = update_and_render_stub;
            engineAPI.processInput = process_input_stub;
        }

    if (!engineAPI.platformInitMemoryBase)
        {
            valid_dll = false;
            engineAPI.platformInitMemoryBase = platform_init_memory_base_stub;
        }
    if (!engineAPI.updateAndRender)
        {
            valid_dll = false;
            engineAPI.updateAndRender = update_and_render_stub;
        }
    if (!engineAPI.processInput)
        {
            valid_dll = false;
            engineAPI.processInput = process_input_stub;
        }

    return dll;
}

void unload_game(HMODULE dll)
{
    FreeLibrary(dll);
    //DeleteFile(COPIED_DLL);
    valid_dll = false;
    engineAPI.platformInitMemoryBase = platform_init_memory_base_stub;
    engineAPI.updateAndRender = update_and_render_stub;
    engineAPI.processInput = process_input_stub;    
}
#endif

global_variable b32 running = true;

#define MAX_KEY_MESSAGES 16
global_variable u64 curr_keyflags_to_set = 0;
global_variable u64 curr_keyflags_to_unset = 0;
global_variable u8  curr_mouseflags_to_set = 0;
global_variable u8  curr_mouseflags_to_unset = MOUSE_MOVE;
global_variable v2  curr_cursor = {640, 360}; // zero2()?

typedef struct Window_rect_dims
{
    s32 width;
    s32 height;
} window_rect_dims;

inline window_rect_dims get_window_rect_dims(HWND window)
{
    RECT window_rect;
    GetClientRect(window, &window_rect);
    window_rect_dims rect = {
        .width = window_rect.right - window_rect.left,
        .height = window_rect.bottom - window_rect.top };
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

void stretch_and_draw_window_buffer(HDC dc, void* window_buffer_memory, BITMAPINFO* window_buffer_info,int window_width, int window_height)
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
                /* window_rect_dims rect = get_window_rect_dims(window); */
                /* realloc_window_bitmap_buffer(rect.width, rect.height); */
            } break;

        case WM_KEYUP:
            {
                curr_keyflags_to_unset |= (u64)(1 << keymap[(u8)wParam]);
            } break;
        case WM_KEYDOWN:
        /* case WM_SYSKEYDOWN: */
        /* case WM_SYSKEYUP: */
            {
                //@Note I need to know key code, is_down, was_down
                // ... and potentially repeat_count and syskey ...
                
                u16 repeat_count = lParam & 0x0000FFFF;

                // @TODO figure out how to map when having more than 64 flags for keys
                curr_keyflags_to_set |= (u64)(1 << keymap[(u8)wParam]);
                
                // curr_frame_key.is_down = (message == WM_KEYDOWN);
                // curr_frame_key.was_down = lParam & 0x40000000;

                /* if ((message == WM_SYSKEYUP && wParam == VK_F4) || (wParam == VK_ESCAPE)) */
                /*     { */
                /*         running = false; */
                /*     } */
            } break;

        case WM_MOUSEMOVE:
            {
                curr_cursor = V2(lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16);
                curr_mouseflags_to_set |= MOUSE_MOVE;
            } break;

            // @TODO figure out if I need to make a cursor for these two (4...)
        case WM_LBUTTONUP:
            {
                curr_cursor = V2(lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16);
                curr_mouseflags_to_unset |= MOUSE_M1;
            } break;
        case WM_LBUTTONDOWN:
            {
                curr_cursor = V2(lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16);
                curr_mouseflags_to_set |= MOUSE_M1;
            } break;
        case WM_RBUTTONUP:
            {
                curr_cursor = V2(lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16);
                curr_mouseflags_to_unset |= MOUSE_M2;
            } break;
        case WM_RBUTTONDOWN:
            {
                curr_cursor = V2(lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16);
                curr_mouseflags_to_set |= MOUSE_M2;
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

        case WM_PAINT: {
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
    // @Failure failure points??
    
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

    int bytes_per_pixel = 4;
    
    u64 total_memory_capacity = gigabytes(4);
    u64 perm_memory_capacity = megabytes(64);
    u64 temp_memory_capacity = total_memory_capacity - perm_memory_capacity;
    void* base_ptr = VirtualAlloc(0, total_memory_capacity, MEM_COMMIT, PAGE_READWRITE);

    // @TODO if not use DLL then we probably have one global platformAPI*,
    // which we want to set directly only once

    platformAPI = (PlatformAPI*) base_ptr;
    
    platformAPI->platformDisplay.bytesPerPixel = bytes_per_pixel;
    /* platformAPI->platformMemory.memoryBase = base_ptr; */
    platformAPI->platformMemory.memoryCapacity = total_memory_capacity;
#if USE_DLL
    platformAPI->platformProcedures.readFile = read_file;
    platformAPI->platformProcedures.writeFile = write_file;
#endif

    u64 permMemCap = megabytes(64);
    u64 tempMemCap = gigabytes(4);

    u8* permMemPtr = ((u8*)(platformAPI) + sizeof(PlatformAPI) + sizeof(EngineMemory));
    u8* tempMemPtr = permMemPtr + permMemCap;
    
    EngineMemory* memory = (EngineMemory*)((u8*)(platformAPI) + sizeof(PlatformAPI));
    memory->perm_mem = permMemPtr;
    memory->temp_mem = tempMemPtr;
    memory->perm_mem_cap = permMemCap;
    memory->temp_mem_cap = tempMemCap;
    
    // @Note the following line should be able to replace the #define
    /* memory_base = (platform_provides*)base_ptr; */
    // #define memory_base ((platform_provides*)base_ptr)
    
    /* platform_provides provides = { */
        /* .perm_mem = (u8*)base_ptr + sizeof(platform_provides), */
        /* .perm_mem_cap = perm_memory_capacity, */
        /* .temp_mem = (u8*)base_ptr + sizeof(platform_provides) + perm_memory_capacity, */
        /* .temp_mem_cap = temp_memory_capacity, */
        /* .bytpp = bytes_per_pixel, */
        /* .dbg_read_file = dbg_read_entire_file, */
        /* .dbg_write_file = dbg_write_entire_file }; */

    /* *((platform_provides*)base_ptr) = provides; */

    PLATFORM_INIT_MEMORY_BASE((PlatformAPI*) base_ptr);

    void* window_buffer_memory = (void*)wnd_buffer;
    
    int window_offset_x = 50;
    int window_offset_y = 50;

    WNDCLASS registered_window_infostruct = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_procedure,
        .hInstance = hInstance,
        .lpszClassName = "CardcrafterWindowClass" };

    ATOM registered_window = RegisterClass(&registered_window_infostruct);

    // @Note initial window dims (perhaps offset too), is not written by
    // platform, but by game and they can be inconsistent, think about
    // whose job it is to do that, and how to resize the window....
    // @TODO window resizing...
    
    HWND window = CreateWindow(
                               registered_window_infostruct.lpszClassName,
                               "Cardcrafter",
                               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                               window_offset_x,
                               window_offset_y,
                               1280 + 16,
                               720 + 39,
                               0,
                               0,
                               hInstance,
                               0
                               );
    
    // default resolution is 1087x584, UL=(0,0)
    BITMAPINFO window_buffer_info = {0};
    window_buffer_info.bmiHeader.biSize = sizeof(window_buffer_info.bmiHeader);
    window_buffer_info.bmiHeader.biPlanes = 1;
    window_buffer_info.bmiHeader.biBitCount = bytes_per_pixel*8;
    window_buffer_info.bmiHeader.biCompression = BI_RGB;

    // SetCursorPos(640 + window_offset_x,
                 // 360 + window_offset_y);

    wnd_width = 1280;
    wnd_height = 720;
    POINT center = { wnd_width/2, wnd_height/2 };
    ClientToScreen(window, &center);
    SetCursorPos(center.x, center.y);
    
    LARGE_INTEGER counter_frequency;
    QueryPerformanceFrequency(&counter_frequency);
    
    while (running)
        {
#if USE_DLL
            // @TODO when doing rotated_line test, then hotloading perspective_test
            // it won't hotload every time, and also sometimes it might bug out
            // and not load anything...?

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
                    
                    /* PLATFORM_INIT_MEMORY_BASE(platformAPI); */
                    PLATFORM_INIT_MEMORY_BASE((PlatformAPI*) base_ptr);
                }
#endif         
            u64 begin_cycle_count = __rdtsc();

            LARGE_INTEGER begin_time_count;
            QueryPerformanceCounter(&begin_time_count);

            // @TODO do this in a while loop actually....
            // actually then you have to somehow remove WM_PAINT
            // from the queue because it is not removed by default....
            // MSG message;
            // PeekMessage(&message, 0, 0, 0, PM_REMOVE);

            // if (message.message == WM_QUIT)
            //     {
            //         running = false;
            //     }

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
            b32 camera_mode = PROCESS_INPUT(curr_keyflags_to_set,
                                            curr_keyflags_to_unset,
                                            curr_mouseflags_to_set,
                                            curr_mouseflags_to_unset,
                                            curr_cursor); // can use get cursor pos instead of messages
            if (camera_mode)
                {
                    ShowCursor(false);
                    SetCursorPos(center.x, center.y);
                }
            else
                {
                    ShowCursor(true);
                }
            UPDATE_AND_RENDER();

            window_buffer_info.bmiHeader.biWidth = wnd_width;
            window_buffer_info.bmiHeader.biHeight = -wnd_height;

            
            // @TODO figure out if I need to have two different types of
            // resizing, one that resizes the buffer, and the other
            // that resizes the window rect, and then if not the same
            // it will stretch
            // @TODO resize window rect??? (AjdustWindowRect)
            
            HDC dc = GetDC(window);
            window_rect_dims rect = get_window_rect_dims(window);
            stretch_and_draw_window_buffer(dc,
                                           window_buffer_memory,
                                           &window_buffer_info,
                                           rect.width,
                                           rect.height);
            ReleaseDC(window, dc);

            curr_keyflags_to_set = 0;
            curr_keyflags_to_unset = 0;
            curr_mouseflags_to_set = 0;
            curr_mouseflags_to_unset = MOUSE_MOVE;

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
 
