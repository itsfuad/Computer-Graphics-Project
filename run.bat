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

echo Linking...
g++.exe -L"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" ^
  -o bin\Debug\test.exe obj\Debug\test.o ^
  -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 -lopenal32

if errorlevel 1 (
  echo Linking failed.
  exit /b 1
)

echo Running...
bin\Debug\test.exe
