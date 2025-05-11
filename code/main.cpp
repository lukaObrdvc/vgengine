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

// #if !USE_DLL
// #define dbg_print(str) OutputDebugString(str);
// #else
// #define dbg_print(str) ((void)0)
// #endif

#include "basic.h"

#include "math/vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "math/transform.h"

#include "rect.h"

#include "arena.h"

#include "camera.h"
// #include "graphics/clip.h"
#include "framebuffer.h"
// #include "line.h"
#include "graphics/pixel.h"
// #include "graphics/renderer.h"


#include "new_mani.h"

#include "platform.h"

#include "init.h"
#include "input.h"

#if !USE_DLL
#include "platform/engine.h"
#endif

// .cpps here probably.....
#include "math/matrix.cpp"
#include "math/quaternion.cpp"

#include "core/arena.cpp"

// #include "graphics/line.cpp"

#include "graphics/framebuffer.cpp"
#include "init.cpp"
#include "input.cpp"

#include "engine.cpp"
#if !USE_DLL
#include "platform/platform.cpp"
#endif
