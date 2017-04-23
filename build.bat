@echo off

set output=app.exe
set Olevel=/Ox
set options=-W3 %Olevel% /std:c++latest

set libs=opengl32.lib glew32s.lib glfw3dll.lib kernel32.lib
del %output%
cl /EHsc %options% -Fe%output% .\src\*.cpp %libs%
del *.obj
