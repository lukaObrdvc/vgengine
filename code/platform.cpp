#include "project.h"

#if USE_DLL

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include "utils.h"
#include "vector.h"

#include "platform.h"
#include "engine.h"
#include "new_mani.h"

#endif

#if PLATFORM_WINDOWS
#include "windows.cpp"
#endif

