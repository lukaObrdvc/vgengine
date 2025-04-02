@echo off

REM -wd4013 is iffy, I don't get it......
REM sometimes useful warnings: 4701
REM temporary -wd4057
set WarningFlags=-W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -wd4013 -wd4101 -wd4715 -wd4701 -wd4057
set CompilerFlags=/std:c17 /Zc:preprocessor /TC -Oi -Zi -MT -GR -EHa -nologo
set LinkerFlags=-incremental:no -opt:ref
set Imports=user32.lib gdi32.lib kernel32.lib winmm.lib
set Exports=/EXPORT:update_and_render /EXPORT:platform_init_memory_base /EXPORT:process_input
set Defines=-DDEBUG=1 -DHOTLOAD=1

pushd ..\exe
cl %CompilerFlags% %WarningFlags% %Defines% -Fmmain.map            ..\code\main.c /LD         /link %LinkerFlags% %Exports%
cl %CompilerFlags% %WarningFlags% %Defines% -Fmwin32_vgengine.map  ..\code\win32_vgengine.c   /link %LinkerFlags% %Imports%
popd
