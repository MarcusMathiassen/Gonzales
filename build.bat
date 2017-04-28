@echo off

set std=/std:c++latest
set output=app.exe
set olvl=
set wlvl=-W3 /wd4530 /wd4005 /wd4996
set options=%std% %wlvl% %olvl% %std%

set src=.\src\*.cpp
set libs=opengl32.lib glew32s.lib glfw3dll.lib user32.lib gdi32.lib kernel32.lib

del %output%

cl /EHsc %options% %src% %libs% -Fe%output%

del *.obj
