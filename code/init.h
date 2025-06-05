#ifndef INIT_H
#define INIT_H

void init_memory();
void init_engine_state();

extern "C" void platform_init_engine();
#if USE_DLL
extern "C" void platform_init_memory_base(Globals* memory_base);
#endif

#endif
