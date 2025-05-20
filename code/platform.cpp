// "W:\\Projects\\vgengine\\code\\"

#include "project.h"

#if USE_DLL

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#include "core/basic.h"
// #include "core/arena.h"

#include "platform/platform.h"
#include "platform/engine.h"

#endif

// this is probably temporarily here
// #include "graphics/framebuffer.h"
// #include "graphics/camera.h"
// #include "new_mani.h"

#if PLATFORM_WINDOWS
#include "platform/windows.cpp"
#endif
