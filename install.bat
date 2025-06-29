cls
@echo off
setlocal enabledelayedexpansion

echo -------------------------------------------------------
echo   MinGW Library Installation Script
echo -------------------------------------------------------

:: Initialize error flags
set error=0
set error_glut_headers=0
set error_openal_headers=0
set error_glut_libs=0
set error_openal_static_libs=0
set error_openal_import_libs=0
set error_openal_executable=0

:: Step 1: Find g++ compiler
echo [1/5] Searching for MinGW compiler...
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

:: Step 2: Detect architecture
echo [2/5] Detecting architecture...
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

:: Step 3: Copy FreeGLUT headers
echo [3/5] Copying FreeGLUT headers...
if not exist "%target_include%\GL" mkdir "%target_include%\GL"
xcopy /y "%~dp0freeglut\include\GL\*.h" "%target_include%\GL\" >nul 2>&1
if %errorlevel% equ 0 (
    echo   FreeGLUT headers copied successfully
) else (
    echo   ERROR: Failed to copy FreeGLUT headers
    echo   Source: %~dp0freeglut\include\GL\
    echo   Destination: %target_include%\GL\
    set error=1
    set error_glut_headers=1
)

:: Step 4: Copy OpenAL headers
echo [4/5] Copying OpenAL headers...
if not exist "%target_include%\AL" mkdir "%target_include%\AL"
xcopy /y "%~dp0OpenAL\include\AL\*.h" "%target_include%\AL\" >nul 2>&1
if %errorlevel% equ 0 (
    echo   OpenAL headers copied successfully
) else (
    echo   ERROR: Failed to copy OpenAL headers
    echo   Source: %~dp0OpenAL\include\AL\
    echo   Destination: %target_include%\AL\
    set error=1
    set error_openal_headers=1
)
echo.

:: Step 5: Copy libraries
echo [5/5] Copying libraries...

:: FreeGLUT libraries
echo   FreeGLUT libraries (%freeglut_lib_dir%)
xcopy /y "%~dp0freeglut\lib\%freeglut_lib_dir%\*.a" "%target_lib%\" >nul 2>&1
rem copy dll to windows system directory
xcopy /y "%~dp0freeglut\bin\%freeglut_lib_dir%\*.dll" "%windir%" >nul 2>&1
if %errorlevel% equ 0 (
    echo     FreeGLUT libraries copied
) else (
    echo     ERROR: Failed to copy FreeGLUT libraries
    echo     Source: %~dp0freeglut\lib\%freeglut_lib_dir%\
    echo     Destination: %target_lib%\
    set error=1
    set error_glut_libs=1
)

:: OpenAL libraries
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
:: Step 6: Install openAL executable
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

:: Final summary
echo.
echo -------------------------------------------------------
if %error% equ 0 (
    echo Installation completed successfully!
) else (
    echo Installation completed with errors.
    echo.

    if %error_glut_headers% equ 1 (
        echo   - FreeGLUT headers failed to copy
    )
    if %error_openal_headers% equ 1 (
        echo   - OpenAL headers failed to copy
    )
    if %error_glut_libs% equ 1 (
        echo   - FreeGLUT libraries failed to copy
    )
    if %error_openal_static_libs% equ 1 (
        echo   - OpenAL static libraries failed to copy
    )
    if %error_openal_import_libs% equ 1 (
        echo   - OpenAL import libraries failed to copy
    )
    if %error_openal_executable% equ 1 (
        echo   - OpenAL executable failed to install
    )

    echo.
    echo Please verify that source directories and files exist.
)
echo -------------------------------------------------------
pause >nul
