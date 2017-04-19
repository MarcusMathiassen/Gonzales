@echo off

set output=app.exe
del %output%
cl /EHsc -W3 -Fe%output% .\src\*.cpp opengl32.lib glew32s.lib glfw3dll.lib
del *.obj
