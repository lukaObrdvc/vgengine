@echo off
setlocal

set OPTIMIZATIONS=0
set DEVELOPER=1
set USE_DLL=1

if "%DEVELOPER%"=="0" set USE_DLL=0

REM -wd4013 is iffy, I don't get it......
REM sometimes useful warnings: 4701 4334 (for bitwise shifting)
REM temporary -wd4057
REM C only flags: /std:c17 /Zc:preprocessor /TC
set WarningFlags=-W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -wd4013 -wd4101 -wd4715 -wd4701 -wd4057 -wd4206 -wd4334
set BaseFlags=-GR -EHa -nologo /std:c++20
set LinkerFlags=-incremental:no -opt:ref
set Imports=user32.lib gdi32.lib kernel32.lib winmm.lib
set Exports=/EXPORT:update_and_render /EXPORT:platform_init_engine /EXPORT:platform_init_memory_base

set Defines=

if "%DEVELOPER%"=="1" (
   set Defines=-DDEVELOPER=1
) else (
   set Defines=-DDEVELOPER=0
)

if "%USE_DLL%"=="1"   (
   set Defines=%Defines% -DUSE_DLL=1
) else (
   set Defines=%Defines% -DUSE_DLL=0
)

set Defines=%Defines% -DWINDOWS=1

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
    cl %CompilerFlags% -Fmmain.map     ..\code\main.cpp /LD /link %LinkerFlags% %Exports%
    cl %CompilerFlags% -Fmplatform.map ..\code\platform.cpp /link %LinkerFlags% %Imports%
) else (
    cl %CompilerFlags% -Fmmain.map     ..\code\main.cpp     /link %LinkerFlags% %Imports%
)

popd

endlocal
