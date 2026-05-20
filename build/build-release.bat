@echo off
setlocal enabledelayedexpansion

if not exist build mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 exit /b %errorlevel%
cmake --build . --config Release
if errorlevel 1 exit /b %errorlevel%

if exist "bin\ControllerEngine.dll" (
    echo Copying ControllerEngine.dll to C# output directory...
    copy /Y "bin\ControllerEngine.dll" "..\src\csharp\bin\Release\net6.0-windows\"
)

echo Release build complete.
