@echo off

pushd ..\exe
REM cl -Zi ..\code\cardcrafter.cpp /link /DLL /EXPORT:update_and_render
cl /std:c17 /Zc:preprocessor /TC -DDEBUG=1  -Oi -Zi -W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -MT -GR -EHa -nologo -Fmwin32_vgengine.map ..\code\win32_vgengine.c /link -opt:ref user32.lib gdi32.lib kernel32.lib winmm.lib
popd
