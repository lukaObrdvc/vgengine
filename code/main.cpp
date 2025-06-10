// "W:\\Projects\\vgengine\\code\\"


// @todo:

// - windows.cpp cleanup

// - strings
// - colors
// - randomness

// winapi stuffs:
// - file handling
// - fonts               (KarminaBold)
// - threads
// - perf counters?
// - recompilation/reloading?

// - fullscreen
// - cursors
// - window priority
// - resizing

// - audio
// - maketh sureth dll thingy worketh ?

// also line and pixel?


// @todo refactor from base to data for all struct fields?

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
#include "core/flags.h"
#include "core/bitfield.h"

#include "platform/platform.h"

#include "core/slice.h"
#include "core/array.h"
#include "core/pool.h"
#include "core/string.h"
#include "core/hash_table.h"


#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/transform.h" // @cleanup

#include "core/rect.h"

#include "graphics/triangle.h" // @cleanup
#include "graphics/camera.h" // @cleanup
#include "graphics/clip.h" // @cleanup
#include "graphics/framebuffer.h" // @cleanup
// #include "graphics/line.h"
// #include "graphics/pixel.h"
#include "graphics/renderer.h" // mesh, color_make


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
// #include "math/transform.cpp"

// #include "graphics/line.cpp"

#include "graphics/framebuffer.cpp"
#include "init.cpp"
#include "input.cpp"

#include "platform/engine.cpp" // @cleanup and make update_and_render special file probably?
#if !USE_DLL
#include "platform.cpp"
#endif
