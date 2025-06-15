# OpenGL Scene Animation Project

This is a Computer Graphics course project implemented using OpenGL and GLUT. The project features multiple interactive 3D scenes with various animations and effects.

## Requirements

### Libraries
- OpenGL
- GLUT (OpenGL Utility Toolkit)
- OpenAL (Open Audio Library)

## Installation
1. Install MinGW or Visual Studio with C++ support
2. Install GLUT:
   - Download freeglut from [freeglut website](https://www.transmissionzero.co.uk/software/freeglut-devel/)
   - Extract and copy the files to your MinGW/Visual Studio directories:
     - `freeglut.dll` → `C:\Windows\System32`
     - `freeglut.lib` → `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\lib`
     - Header files → `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\include\GL`

3. Install OpenAL:
   - Download OpenAL SDK from [OpenAL website](https://openal.org/downloads/)
   - Follow the installation instructions for your system

## Project Structure
- `masud.h` - Contains scene implementations and animations
- `fuad.h` - Contains additional scene implementations
- `main.cpp` - Main entry point and window management

## Notes
- The project uses GLUT for window management and OpenGL for rendering
- OpenAL is used for audio effects
- The scenes include various animated objects and interactive elements 