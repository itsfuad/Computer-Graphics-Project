# OpenGL Scene Animation Project

This is a Computer Graphics course project implemented using OpenGL and GLUT. The project features multiple interactive 3D scenes with various animations and effects.


![cover](https://github.com/user-attachments/assets/08adb760-ec46-497b-8728-37a539146e1b)

## Requirements

### Libraries
- OpenGL
- GLUT (OpenGL Utility Toolkit)
- OpenAL (Open Audio Library)

## Installation
1. Install MinGW

### Libraries Setup (Manual Steps)
1. Install GLUT:
   - Copy the files to your MinGW include, lib directories

2. Install OpenAL:
   - Copy the OpenAL files to your MinGW include, lib directories
   - Run the oalinst.exe to install OpenAL on your system. ( find the .exe inside the OpenAL folder )

### Automated Installation
Run the `install.bat` script in the project root directory. This script will:
- Search for the MinGW compiler
- Copy necessary library files for OpenGL, GLUT, and OpenAL
- Install OpenAL if the installer is found
- Verify the installation and report any errors

## Notes
- The project uses GLUT for window management and OpenGL for rendering
- OpenAL is used for audio effects
- The scenes include various animated objects and interactive elements 
