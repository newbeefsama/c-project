@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo ==========================================
echo   Compiling my_string.c + test.c ...
echo ==========================================

"D:\w64devkit\bin\gcc.exe" -Wall -Wextra my_string.c test.c -o test.exe

if errorlevel 1 (
    echo.
    echo  *** COMPILE FAILED - read the errors above ***
    echo.
    pause
    exit /b 1
)

echo   Compile OK.
echo ------------------------------------------
test.exe
echo ------------------------------------------
echo.
echo Press any key to close this window.
pause >nul
