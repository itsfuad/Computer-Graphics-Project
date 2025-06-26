cls
@echo off

echo Compiling...
g++.exe -std=c++17 -Wall -g -I"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\include" -c main.cpp -o obj\Debug\main.o

if errorlevel 1 (
  echo Compilation failed.
  exit /b 1
)

echo Compiled