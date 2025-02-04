@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

pushd ..\exe
REM cl -Zi ..\code\cardcrafter.cpp /link /DLL /EXPORT:update_and_render
cl -DDEBUG=1 -Oi -Zi -W4 -WX -wd4100 -wd4091 -wd4459 -wd4996 -wd4189 -wd4201 -wd4804 -wd4244 -MT -GR -EHa -nologo -Fmwin32_cardcrafter.map ..\code\win32_cardcrafter.cpp /link -opt:ref user32.lib gdi32.lib kernel32.lib winmm.lib
popd
