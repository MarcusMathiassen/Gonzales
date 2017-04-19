@echo off

set output=MM.exe
del %output%
cl /EHsc -W3 -Fe%output% *.cpp opengl32.lib glew32s.lib glfw3dll.lib
del *.obj
