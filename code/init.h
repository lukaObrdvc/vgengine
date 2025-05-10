#ifndef INIT_H
#define INIT_H

#if USE_DLL
void platform_init_memory_base(Globals* memoryBase);
#endif

void init_memory();
void init_engine_state();


#endif
