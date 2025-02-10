@echo off

REM -wd4013 is iffy, I don't get it......
set WarningFlags=-W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -wd4013
set CompilerFlags=/std:c17 /Zc:preprocessor /TC -DDEBUG=1 -Oi -Zi -MT -GR -EHa -nologo
set LinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib kernel32.lib winmm.lib
set DLLExports=/EXPORT:update_and_render /EXPORT:platform_init_game /EXPORT:process_frame_input
REM set UniquePDBperDLL=/PDB:main_%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%.pdb

pushd ..\exe
REM del *.pdb > NUL 2> NUL             %UniquePDBperDLL%
cl %CompilerFlags% %WarningFlags% -Fmmain.map ..\code\main.c /LD /link -incremental:no -opt:ref %DLLExports%
cl %CompilerFlags% %WarningFlags% -Fmwin32_vgengine.map ..\code\win32_vgengine.c /link %LinkerFlags%
popd
