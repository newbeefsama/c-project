@echo off
chcp 65001 >nul
cd /d "%~dp0"

rem ===== pick source file =====
rem drag a .c file onto this bat  -> compile that file
rem double-click                  -> compile main.c
set SRC=%~f1
if "%SRC%"=="" set SRC=%~dp0main.c

if not exist "%SRC%" (
    echo.
    echo   No main.c in this folder.  Two options:
    echo     1^) rename your code file to main.c
    echo     2^) drag a .c file onto this bat
    echo.
    pause
    exit /b 1
)

for %%f in ("%SRC%") do set OUT=%%~dpnf.exe
for %%f in ("%SRC%") do set NAME=%%~nxf

echo ==========================================
echo   Compiling %NAME% ...
echo ==========================================
"D:\w64devkit\bin\gcc.exe" -Wall "%SRC%" -o "%OUT%"

if errorlevel 1 (
    echo.
    echo  *** COMPILE FAILED - read the errors above ***
    echo.
    pause
    exit /b 1
)

echo   Compile OK. Running:
echo ------------------------------------------
"%OUT%"
echo ------------------------------------------
echo.
echo Press any key to close this window.
pause >nul
