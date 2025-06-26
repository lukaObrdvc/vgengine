// "W:\\Projects\\vgengine\\code\\"

// roadmap:

// - profiling system
// - logging system
// - UI stuff

// - threading
// - SIMD

// - shading
// - texturing

// - mesh loader
// - texture loader
// - scene loader

// - audio

// - console?
// - serialization?
// - simple editor?
// - introspection?

// - 3d animation?
// - anti-aliasing?


// need from WinAPI:
// - threads
// - recompilation/reloading?
// - (GetSystemTime, WORD=u16)


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
#include "core/scratch.h"
#include "core/sort.h"
#include "core/flags.h"
#include "core/bit_array.h"


#include "platform/platform.h"

#include "core/slice.h"
#include "core/array.h"
#include "core/pool.h"
#include "core/string.h" // @todo implement a bunch of functionality for this
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
#include "platform.cpp"
#endif
