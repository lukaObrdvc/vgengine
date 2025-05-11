#ifndef PROJECT_H
#define PROJECT_H

#define PLATFORM_WINDOWS 1

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


#endif
