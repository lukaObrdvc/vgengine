// "W:\\Projects\\vgengine\\code\\"


// @todo:
// windows.cpp cleanup
// - strings
// - hash table
// - bitfield
// - colors
// - array slice?
// - sorting
// - randomness

// winapi stuffs:
// - file handling
// - fonts
// - threads
// - fullscreen
// - cursors
// - window priority
// - resizing
// - perf counters?
// - recompilation/reloading?
// - audio
// - maketh sureth dll thingy worketh ?


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

#include "platform/platform.h"

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
