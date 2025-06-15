cls
@echo off

echo Compiling...
g++.exe -Wall -g ^
  -I"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\include" ^
  -c D:\dev\OpenGL\man_walk\test.cpp -o obj\Debug\test.o

if errorlevel 1 (
  echo Compilation failed.
  exit /b 1
)

echo Compiled