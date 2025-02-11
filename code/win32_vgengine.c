#include "Windows.h"
#include "Memoryapi.h"
#include "DSound.h"

#include "stdio.h"
#include "intrin.h"
#include "malloc.h" // @TODO do I need this?

#include "utils.h"
#include "main.h"
//#include "main.c"

global_variable b32 running = true;
global_variable b32 valid_dll = false;


void init_memory_base_stub(void* memory_base)
{
}
typedef void (*fp_init_memory_base) (void* memory_base);
fp_init_memory_base init_memory_base = init_memory_base_stub;

void* update_and_render_stub()
{
    return 0;
}
typedef void* (*fp_update_and_render) ();
fp_update_and_render update_and_render = update_and_render_stub;

void process_frame_input_stub(key k, b32 tk, mouse m, b32 tm)
{
}
typedef void (*fp_process_frame_input) (key k, b32 tk, mouse m, b32 tm);
fp_process_frame_input process_frame_input = process_frame_input_stub;

HMODULE load_game()
{
    CopyFile("W:\\vgengine\\exe\\main.dll", "W:\\vgengine\\exe\\main_copy.dll", FALSE);
    HMODULE dll = LoadLibrary("W:\\vgengine\\exe\\main_copy.dll");

    if (dll)
        {
            valid_dll = true;
            init_memory_base = (fp_init_memory_base) GetProcAddress(dll, "platform_init_memory_base");
            update_and_render = (fp_update_and_render) GetProcAddress(dll, "update_and_render");
            process_frame_input = (fp_process_frame_input) GetProcAddress(dll, "process_frame_input");
        }
    else
        {
            valid_dll = false;
            init_memory_base = init_memory_base_stub;
            update_and_render = update_and_render_stub;
            process_frame_input = process_frame_input_stub;
        }

    if (!init_memory_base)
        {
            valid_dll = false;
            init_memory_base = init_memory_base_stub;
        }
    if (!update_and_render)
        {
            valid_dll = false;
            update_and_render = update_and_render_stub;
        }
    if (!process_frame_input)
        {
            valid_dll = false;
            process_frame_input = process_frame_input_stub;
        }

    return dll;
}

void unload_game(HMODULE dll)
{
    FreeLibrary(dll);
    valid_dll = false;
    init_memory_base = init_memory_base_stub;
    update_and_render = update_and_render_stub;
    process_frame_input = process_frame_input_stub;
}

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

// @TODO: make a (macro trick) scope here for closing handle and returning....
// @TODO: returning with error code ??
b32 dbg_read_entire_file(u8* filename, void* buffer, u32* buffer_size)
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

b32 dbg_write_entire_file(u8* filename, void* buffer, u32 buffer_size)
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

/* void realloc_window_bitmap_buffer(int new_width, int new_height) */
/* { */
/*     // @Note memory, wndbuffer, origin_x, origin_y are globals */
/*     if (memory.size) */
/*         { */
/*             memory.size = 0; */
/*         } */

/*     origin_x = new_width / 2; */
/*     origin_y = new_height / 2; */
    
/*     wndbuffer.width = new_width; */
/*     wndbuffer.height = new_height; */

/*     window_buffer_info.bmiHeader.biWidth = new_width; */
/*     window_buffer_info.bmiHeader.biHeight = -new_height; */
  
/*     s32 new_size = new_width*new_height*wndbuffer.bytpp; */
/*     wndbuffer.memory = (u8*) memory.base; // @Note this line is not needed */
/*     memory.size = new_size; */
/* } */

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

    b32 used_key = false;
    b32 used_mouse = false;
    key curr_frame_key = {0};
    mouse curr_frame_mouse = {0};
    
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

        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            {
                // @Note just let windows figure out info about
                // keys, include a header, then just have one
                // call from game layer where you pass it
                // all relevant infomation at the end

                //@Note I need to know key code, is_down, was_down
                // ... and potentially repeat_count and syskey ...
                
                //u16 repeat_count = lParam & 0x0000FFFF;
                
                // @Note frame_key is a global

                switch(wParam)
                    {
                    case VK_UP:
                        {
                            curr_frame_key.code = KEY_UP;
                        } break;
                    case VK_DOWN:
                        {
                            curr_frame_key.code = KEY_DOWN;
                        } break;
                    case VK_RIGHT:
                        {
                            curr_frame_key.code = KEY_RIGHT;
                        } break;
                    case VK_LEFT:
                        {
                            curr_frame_key.code = KEY_LEFT;
                        } break;
                    default:
                        {}                      
                    }

                curr_frame_key.is_down = (message == WM_KEYDOWN);
                curr_frame_key.was_down = lParam & 0x40000000;

                used_key = true;
                
                if ((message == WM_SYSKEYUP && wParam == VK_F4) || (wParam == VK_ESCAPE))
                    {
                        running = false;
                    }
            } break;

        case WM_MOUSEMOVE:
            {
                // @Note cursor_, mouse_frame_key is a global
                curr_frame_mouse.cursor = literal(v2) { .x =lParam & 0x0000FFFF,
                                                        .y =(lParam & 0xFFFF0000) >> 16 };
                curr_frame_mouse.mouse_moved = true;

                used_mouse = true;
                // @Fail what if you get WM_MOUSEMOVE after WM_LBUTTONDOWN, and you reset the mouse_key.code but
                // you need to change that code value maybe ??
            } break;
            
        case WM_LBUTTONDOWN:
            //case WM_LBUTTONUP:
            {
                curr_frame_mouse.cursor = literal(v2) { .x =lParam & 0x0000FFFF,
                                                        .y =(lParam & 0xFFFF0000) >> 16 };

                curr_frame_mouse.code = M1;
                curr_frame_mouse.is_down = true;

                used_mouse = true;
            } break;

        case WM_RBUTTONDOWN:
            //case WM_RBUTTONUP:
            {
                curr_frame_mouse.code = M2;
                curr_frame_mouse.is_down = true;

                used_mouse = true;
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

        case WM_PAINT: {} break;
            
        default:
            {
                result = DefWindowProc(window, message, wParam, lParam);
            }            
        }

    // @IMPORTANT I have no idea how these messages work, whether
    // they will process multiple messages at once
    process_frame_input(curr_frame_key,
                        used_key,
                        curr_frame_mouse,
                        used_mouse);
    
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nShowCmd)
{
    // @Failure failure points??
    
    timeBeginPeriod(1);

    HMODULE dll = load_game();
    int dll_reload_counter = 0;

    int bytes_per_pixel = 4;
    
    u64 total_memory_capacity = gigabytes(4);
    u64 perm_memory_capacity = megabytes(64);
    u64 temp_memory_capacity = total_memory_capacity - perm_memory_capacity;
    void* base_ptr = VirtualAlloc(0, total_memory_capacity, MEM_COMMIT, PAGE_READWRITE);

    #define memory_base ((platform_provides*)base_ptr)    
    
    platform_provides provides = {
        .perm_mem = (u8*)base_ptr + sizeof(platform_provides),
        .perm_mem_cap = perm_memory_capacity,
        .temp_mem = (u8*)base_ptr + sizeof(platform_provides) + perm_memory_capacity,
        .temp_mem_cap = temp_memory_capacity,
         .bytpp = bytes_per_pixel,
        .dbg_read_file = dbg_read_entire_file,
        .dbg_write_file = dbg_write_entire_file };

    *((platform_provides*)base_ptr) = provides;

    init_memory_base(base_ptr);

    void* window_buffer_memory = (void*)wnd_buffer;
    
    int window_offset_x = 50;
    int window_offset_y = 50;
    
    WNDCLASS registered_window_infostruct = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_procedure,
        .hInstance = hInstance,
        .lpszClassName = "CardcrafterWindowClass" };

    ATOM registered_window = RegisterClass(&registered_window_infostruct);

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
    BITMAPINFO window_buffer_info = {
        .bmiHeader.biSize = sizeof(window_buffer_info.bmiHeader),
        .bmiHeader.biPlanes = 1,
        .bmiHeader.biBitCount = bytes_per_pixel*8,
        .bmiHeader.biCompression = BI_RGB };
    
    LARGE_INTEGER counter_frequency;
    QueryPerformanceFrequency(&counter_frequency);
    
    while (running)
        {
#if HOTLOAD // @TODO reload based on file datetime instead, to remove stuttering
            if (dll_reload_counter >= 30)
                {
                    unload_game(dll);
                    dll = load_game();
                    dll_reload_counter = 0;

                    init_memory_base(base_ptr);
                }
#endif         
            u64 begin_cycle_count = __rdtsc();

            LARGE_INTEGER begin_time_count;
            QueryPerformanceCounter(&begin_time_count);
            
            MSG message;
            PeekMessage(&message, 0, 0, 0, PM_REMOVE);

            if (message.message == WM_QUIT)
                {
                    running = false;
                }
            
            TranslateMessage(&message);
            DispatchMessage(&message);

            update_and_render();
            
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
            //OutputDebugStringA(debug_str);

            dll_reload_counter++;

            
            //Sleep(200);
        }

    return 0;
}
 
