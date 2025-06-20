// "W:\\Projects\\vgengine\\code\\"


#if USE_DLL

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#include "project.h"

#include "core/basic.h"
#include "core/arena.h"
#include "core/scratch.h"
#include "core/flags.h"
#include "core/bit_array.h"

#include "input.h"
#include "update_and_render.h"
#include "platform/platform.h"
#include "init.h"
#include "platform/engine.h"

#endif

#if WINDOWS
#include "platform/windows.cpp"
#endif
