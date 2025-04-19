@echo off
setlocal

set OPTIMIZATIONS=0
set DEVELOPER=1
set USE_DLL=1

if "%DEVELOPER%"=="0" set USE_DLL=0

REM -wd4013 is iffy, I don't get it......
REM sometimes useful warnings: 4701
REM temporary -wd4057
set WarningFlags=-W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -wd4013 -wd4101 -wd4715 -wd4701 -wd4057 -wd4206
set BaseFlags=/std:c17 /Zc:preprocessor /TC -GR -EHa -nologo
set LinkerFlags=-incremental:no -opt:ref
set Imports=user32.lib gdi32.lib kernel32.lib winmm.lib
set Exports=/EXPORT:update_and_render /EXPORT:process_input

set Defines=

if "%DEVELOPER%"=="1" (
   set Defines=-DBUILD_DEVELOPER=1
) else (
   set Defines=-DBUILD_DEVELOPER=0
)

if "%USE_DLL%"=="1"   (
   set Defines=%Defines% -DBUILD_USE_DLL=1
   set Exports=%Exports% /EXPORT:platform_init_memory_base
) else (
   set Defines=%Defines% -DBUILD_USE_DLL=0
)

REM I might change this a bit...
REM you can add for more optimizations -Ox -GL -Gy
if "%OPTIMIZATIONS%"=="1" (
    set BaseFlags=%BaseFlags% -Oi -O2 -MT
) else (
    set BaseFlags=%BaseFlags% -Oi -Od -Zi -MTd
)

set CompilerFlags=%BaseFlags% %Defines% %WarningFlags%

pushd ..\exe

del /Q *.obj *.pdb *.dll *.exe >nul 2>nul
if "%USE_DLL%"=="1" (
    cl %CompilerFlags% -Fmmain.map     ..\code\main.c /LD /link %LinkerFlags% %Exports%
    cl %CompilerFlags% -Fmplatform.map ..\code\platform.c /link %LinkerFlags% %Imports%
) else (
    cl %CompilerFlags% -Fmmain.map     ..\code\main.c     /link %LinkerFlags% %Imports%
)

popd

endlocal
