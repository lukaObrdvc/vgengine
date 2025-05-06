#include "project.h"
#if !USE_DLL
#if PLATFORM_WINDOWS

#include <Windows.h>
// #undef min
// #undef max

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

#include "utils.h"
#include "basic.h"
#include "Allocator.h"

#include "math.h"
#include "vector.h"
#include "linear_algebra.h"
#include "pixel.h"
#include "renderer.h"

#include "MeshLoader.h"

#include "new_mani.h"


#include "platform.h"
#if !USE_DLL
#include "engine.h"
#endif

#include "engine.cpp"
#if !USE_DLL
#include "platform.cpp"
#endif
