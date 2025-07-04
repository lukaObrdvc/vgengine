#ifndef PROJECT_H
#define PROJECT_H

#define PROJECT_PATH "W:\\Projects\\vgengine\\"

#define DATA_PATH  PROJECT_PATH "data\\"
#define CODE_PATH  PROJECT_PATH "code\\"
#define BUILD_PATH PROJECT_PATH "exe\\"
#define ASSET_PATH DATA_PATH    "assets\\"
#define LOG_PATH   DATA_PATH    "logs\\"
#define MESH_PATH  ASSET_PATH   "meshes\\"
#define FONT_PATH  ASSET_PATH   "fonts\\"

#define ACTUAL_DLL BUILD_PATH "main.dll"
#define COPIED_DLL BUILD_PATH "main_copy.dll" // because windows locks the file for writing when it's loaded

#define CORE_PATH     CODE_PATH "core\\"
#define GRAPHICS_PATH CODE_PATH "graphics\\"
#define MATH_PATH     CODE_PATH "math\\"
#define PLATFORM_PATH CODE_PATH "platform\\"

#define MAKE_FONT_BMP 1
#define FONT_NAME "Consolas"
#define FONT_SIZE 8

#endif
