// "W:\\Projects\\vgengine\\code\\"


// @todo:

// - windows.cpp cleanup [start this next probably :) ]

// - strings

// - fullscreen
// - cursors
// - window priority
// - resizing

// winapi stuffs:
// - read datetime
// - file handling
// - fonts               (KarminaBold)
// - perf counters?

// - threads
// - recompilation/reloading?

// - audio
// - maketh sureth dll thingy worketh ?

// also line and pixel?

#include "project.h"

#if !USE_DLL

#if PLATFORM_WINDOWS
#include <Windows.h>
#endif

#endif

#include <stdarg.h> // for variadic functions
#include <stdio.h>  // for sprintf
#include <stdint.h> // for integral types
#include <math.h>   // for sqrt, sin, cos, tan  (anything else?)


#include "core/basic.h"
#include "core/arena.h"
#include "core/scratch.h"
#include "core/sort.h"
#include "core/flags.h"
#include "core/bit_array.h"

#include "platform/platform.h"

#include "core/slice.h"
#include "core/array.h"
#include "core/pool.h"
#include "core/string.h"
#include "core/hash_table.h" // @todo rehashing

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/transform.h"

#include "core/rect.h"

#include "graphics/color.h"
#include "graphics/mesh.h"
#include "graphics/triangle.h" // @cleanup
#include "graphics/camera.h" // @cleanup
#include "graphics/clip.h" // @cleanup
#include "graphics/framebuffer.h" // @cleanup
// #include "graphics/line.h"
// #include "graphics/pixel.h"

#include "input.h"
#include "engine_state.h"
#include "core/random.h" // move this later (needs prng_state from ENGINE_STATE)

#if !USE_DLL
#include "platform/engine.h"
#endif
#include "update_and_render.h"

#include "init.h"

#include "core/sort.cpp"
#include "input.cpp"
#include "core/arena.cpp"
#include "core/scratch.cpp"

#include "math/matrix.cpp"
#include "math/quaternion.cpp"

// #include "graphics/line.cpp"

#include "graphics/framebuffer.cpp"
#include "init.cpp" // @cleanup

#include "platform/engine.cpp" // @cleanup
#include "test2.cpp"
#include "update_and_render.cpp"
#if !USE_DLL
#include "platform.cpp"
#endif
