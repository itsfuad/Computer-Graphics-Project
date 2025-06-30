@echo off
setlocal enabledelayedexpansion
cls

echo -------------------------------------------------------
echo   MinGW Library Installation Script
echo -------------------------------------------------------

:: Selection menu
echo Select what to install:
echo   [1] FreeGLUT only
echo   [2] OpenAL only
echo   [3] Both
set /p choice=Enter choice [1-3]:

if "%choice%"=="1" (
    set install_freeglut=1
    set install_openal=0
) else if "%choice%"=="2" (
    set install_freeglut=0
    set install_openal=1
) else if "%choice%"=="3" (
    set install_freeglut=1
    set install_openal=1
) else (
    echo Invalid choice. Exiting.
    pause
    exit /b 1
)

:: Initialize error flags
set error=0
set error_glut_headers=0
set error_openal_headers=0
set error_glut_libs=0
set error_openal_static_libs=0
set error_openal_import_libs=0
set error_openal_executable=0

:: Find g++ compiler
echo Searching for MinGW compiler...
where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: g++ not found in PATH
    pause
    exit /b 1
)

for /f "delims=" %%a in ('where g++') do set "gpp_path=%%~a"
echo Found g++ at: %gpp_path%

:: Determine MinGW root
set "mingw_root=%gpp_path:\bin\g++.exe=%"
echo MinGW root directory: %mingw_root%\

:: Detect architecture
echo Detecting architecture...
for /f %%a in ('"%gpp_path%" -dumpmachine') do set "arch=%%a"
echo Architecture raw: [%arch%]

if "%arch%"=="x86_64-w64-mingw32" (
    echo Detected 64-bit architecture
    set "openAL_lib_dir=Win64"
    set "freeglut_lib_dir=x64"
) else (
    echo Detected 32-bit architecture
    set "openAL_lib_dir=Win32"
    set "freeglut_lib_dir=."
)

:: Set target paths
set "target_dir=%mingw_root%\%arch%"
set "target_include=%target_dir%\include"
set "target_lib=%target_dir%\lib"

echo Target directory: %target_dir%
echo Target include: %target_include%
echo Target lib: %target_lib%
echo.

:: Ensure directories exist
if not exist "%target_include%" mkdir "%target_include%"
if not exist "%target_lib%" mkdir "%target_lib%"

:: Copy FreeGLUT headers
if "%install_freeglut%"=="1" (
    echo Copying FreeGLUT headers...
    if not exist "%target_include%\GL" mkdir "%target_include%\GL"
    xcopy /y "%~dp0freeglut\include\GL\*.h" "%target_include%\GL\" >nul 2>&1
    if %errorlevel% equ 0 (
        echo   FreeGLUT headers copied successfully
    ) else (
        echo   ERROR: Failed to copy FreeGLUT headers
        set error=1
        set error_glut_headers=1
    )
)

:: Copy OpenAL headers
if "%install_openal%"=="1" (
    echo Copying OpenAL headers...
    if not exist "%target_include%\AL" mkdir "%target_include%\AL"
    xcopy /y "%~dp0OpenAL\include\AL\*.h" "%target_include%\AL\" >nul 2>&1
    if %errorlevel% equ 0 (
        echo   OpenAL headers copied successfully
    ) else (
        echo   ERROR: Failed to copy OpenAL headers
        set error=1
        set error_openal_headers=1
    )
    echo.
)

:: Copy libraries...
echo Copying libraries...

:: FreeGLUT
if "%install_freeglut%"=="1" (
    echo   FreeGLUT libraries (%freeglut_lib_dir%)
    xcopy /y "%~dp0freeglut\lib\%freeglut_lib_dir%\*.a" "%target_lib%\" >nul 2>&1
    xcopy /y "%~dp0freeglut\bin\%freeglut_lib_dir%\*.dll" "%windir%" >nul 2>&1
    if %errorlevel% equ 0 (
        echo     FreeGLUT libraries copied
    ) else (
        echo     ERROR: Failed to copy FreeGLUT libraries
        set error=1
        set error_glut_libs=1
    )
)

:: OpenAL
if "%install_openal%"=="1" (
    echo   OpenAL libraries (%openAL_lib_dir%)
    xcopy /y "%~dp0OpenAL\libs\%openAL_lib_dir%\*.a" "%target_lib%\" >nul 2>&1
    if %errorlevel% equ 0 (
        echo     OpenAL static libraries copied
    ) else (
        echo     ERROR: Failed to copy OpenAL static libraries
        set error=1
        set error_openal_static_libs=1
    )

    xcopy /y "%~dp0OpenAL\libs\%openAL_lib_dir%\*.lib" "%target_lib%\" >nul 2>&1
    if %errorlevel% equ 0 (
        echo     OpenAL import libraries copied
    ) else (
        echo     ERROR: Failed to copy OpenAL import libraries
        set error=1
        set error_openal_import_libs=1
    )
)

:: Step 6: Install openAL executable
if "%install_openal%"=="1" (
    echo [6/6] Installing OpenAL executable...
    if exist "%~dp0OpenAL\oalinst.exe" (
        echo   Running OpenAL installer...
        "%~dp0OpenAL\oalinst.exe" /S >nul 2>&1
        if %errorlevel% equ 0 (
            echo     OpenAL installed successfully
        ) else (
            echo     ERROR: Failed to install OpenAL
            set error=1
            set error_openal_executable=1
        )
    ) else (
        echo   WARNING: OpenAL installer not found at %~dp0OpenAL\oalinst.exe
    )
)

:: Final summary
echo.
echo -------------------------------------------------------
if %error% equ 0 (
    echo Installation completed successfully!
) else (
    echo Installation completed with errors.
    if %error_glut_headers% equ 1 echo   - FreeGLUT headers failed to copy
    if %error_openal_headers% equ 1 echo   - OpenAL headers failed to copy
    if %error_glut_libs% equ 1 echo   - FreeGLUT libraries failed to copy
    if %error_openal_static_libs% equ 1 echo   - OpenAL static libraries failed to copy
    if %error_openal_import_libs% equ 1 echo   - OpenAL import libraries failed to copy
    if %error_openal_executable% equ 1 echo   - OpenAL executable failed to install
    echo.
    echo Please verify that source directories and files exist.
)
echo -------------------------------------------------------
pause >nul
