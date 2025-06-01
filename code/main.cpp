// "W:\\Projects\\vgengine\\code\\"

#include "project.h"

#if !USE_DLL

#if PLATFORM_WINDOWS
#include <Windows.h>
#endif

#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
// @todo math intrinsics
// for sqrt, sin, cos, (winapi?) signbit
#include <math.h>

#if !USE_DLL
#define dbg_print(str) OutputDebugString(str);
#else
#define dbg_print(str) ((void)0)
#endif

#include "core/basic.h"

#include "core/arena.h"
#include "core/scratch.h"

#include "platform/platform.h"

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/transform.h"

#include "core/rect.h"

#include "graphics/camera.h"
// #include "graphics/clip.h"
#include "graphics/framebuffer.h"
// #include "graphics/line.h"
// #include "graphics/pixel.h"
#include "graphics/renderer.h"


#include "new_mani.h"

#if !USE_DLL
#include "platform/engine.h"
#endif

#include "init.h"
#include "input.h"

// .cpps here probably.....
#include "core/arena.cpp"
#include "core/scratch.cpp"

#include "math/matrix.cpp"
#include "math/quaternion.cpp"

// #include "graphics/line.cpp"

#include "graphics/framebuffer.cpp"
#include "init.cpp"
#include "input.cpp"

#include "platform/engine.cpp"
#if !USE_DLL
#include "platform.cpp"
#endif
