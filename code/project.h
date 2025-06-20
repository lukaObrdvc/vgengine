#ifndef PROJECT_H
#define PROJECT_H

// @todo wrap around USE_WINDOWS, and then place proper stuff in .bat .sh
#define PLATFORM_WINDOWS 1

// @todo should probably not use these tho?

#if BUILD_DEVELOPER
#define DEVELOPER 1
#else
#define DEVELOPER 0
#endif

#if BUILD_USE_DLL
#define USE_DLL 1
#else
#define USE_DLL 0
#endif

#define PROJECT_PATH              "W:\\Projects\\vgengine\\"
#define DATA_PATH    PROJECT_PATH "data\\"
#define CODE_PATH    PROJECT_PATH "code\\"
#define BUILD_PATH   PROJECT_PATH "exe\\"
#define ASSET_PATH   DATA_PATH    "assets\\"
#define LOG_PATH     DATA_PATH    "logs\\"
#define MESHES_PATH  ASSET_PATH   "meshes\\"
#define FONTS_PATH   ASSET_PATH   "fonts\\"

#define ACTUAL_DLL BUILD_PATH "main.dll"
#define COPIED_DLL BUILD_PATH "main_copy.dll"

#define CORE_PATH     CODE_PATH "core\\"
#define GRAPHICS_PATH CODE_PATH "graphics\\"
#define MATH_PATH     CODE_PATH "math\\"
#define PLATFORM_PATH CODE_PATH "platform\\"

#define MAKE_FONT_BMP 0
#define FONT_NAME "Consolas"


#endif
