cls
@echo off

echo Compiling...
g++.exe -Wall -g ^
  -I"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\include" ^
  -c D:\dev\OpenGL\project\main.cpp -o obj\Debug\main.o

if errorlevel 1 (
  echo Compilation failed.
  exit /b 1
)

echo Compiled