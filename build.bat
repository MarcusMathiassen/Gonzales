@echo off

set std=/std:c++latest
set output=athi.exe
set olvl=
set wlvl=-W3 /wd4530 /wd4005 /wd4996
set options=%std% %wlvl% %olvl% %std%
set src=.\src\*.cpp
set libs=opengl32.lib .\dep\Windows\glew32s.lib .\dep\Windows\glfw3dll.lib user32.lib gdi32.lib kernel32.lib
set includes=/I .\dep\Windows\

del %output%

cl /EHsc %options% %includes% %src% %libs% -Fe%output%

del *.obj
