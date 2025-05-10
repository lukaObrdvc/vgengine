#include "project.h"
#include "alias.h"

#if USE_DLL

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#include "basic.h"
#include "arena.h"
#include "vector.h"
#include "linear_algebra.h"
#include "matrix.h"
#include "quaternion.h"

#include "platform.h"
#include "engine.h"
#include "new_mani.h"

#endif

#if PLATFORM_WINDOWS
#include "windows.cpp"
#endif

