// "W:\\Projects\\vgengine\\code\\"

// roadmap:

// - threading
// - UI

// - shading
// - texturing

// - mesh loader
// - texture loader
// - scene loader

// - audio

// - serialization?
// - console?

// - 3d animation?
// - anti-aliasing?
// - simple editor? (need raycasting) (but also maybe things like creating, deleting entities, undo/redo)
// - SIMD?



// - recompilation/reloading? (from WinAPI)


// @todo rename exe files to vgengine.exe


#if !USE_DLL

#if WINDOWS
#include <Windows.h>
#endif

#endif

#include <stdarg.h> // for variadic functions
#include <stdio.h>  // for sprintf
#include <stdint.h> // for integral types
#include <math.h>   // for sqrt, sin, cos, tan  (anything else?)

#include "project.h"

#include "core/basic.h"
#include "core/arena.h"
// @todo make a scratch (pool) per thread...
#include "core/scratch.h" // @todo refactor to use single scratch instead of a pool? (and then you don't even need to use a scratch actually, just use an arena...!!)
#include "core/sort.h"
#include "core/flags.h"
#include "core/bit_array.h"
#include "core/time.h"

#include "thread.h"

#include "platform/platform.h"

#include "core/slice.h"
#include "core/array.h"
#include "core/pool.h"
#include "core/string.h"
#include "core/hash_table.h" // @todo rehashing

#include "profiling.h"

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/transform.h"

#include "core/rect.h"

#include "graphics/color.h"
#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "graphics/framebuffer.h"

#include "loaders/font_loader.h"

// #include "graphics/line.h" // @todo refactor this to work now

#include "input.h"
#include "engine_state.h"
#include "core/random.h"

#include "graphics/software_renderer.h"
#if !USE_DLL
#include "platform/engine.h"
#endif
#include "update_and_render.h"

#include "init.h"

#include "profiling.cpp" // @todo rename to profiler.cpp/.h

#include "core/sort.cpp"
#include "input.cpp"
#include "core/arena.cpp"
#include "core/scratch.cpp"
#include "core/string.cpp"

#include "thread.cpp"

#include "math/matrix.cpp"
#include "math/quaternion.cpp"

// #include "graphics/line.cpp" // @todo refactor this to work now

#include "graphics/framebuffer.cpp"

#include "graphics/software_renderer.cpp"
#include "loaders/font_loader.cpp" // @todo rename this to font.cpp and font.h instead... probably..

#include "init.cpp"

#include "tests/test2.cpp"
#include "update_and_render.cpp"
#if !USE_DLL
#include "platform.cpp" // entry point of the program
#endif
