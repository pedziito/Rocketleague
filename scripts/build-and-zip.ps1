# Build C++ and C# and create a ZIP package (Windows PowerShell)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Write-Host "Configuring CMake (Visual Studio generator)..."
cmake -S build -B build\cmake-out -G "Visual Studio 17 2022" -A x64

Write-Host "Building C++ DLL (Release)..."
cmake --build build\cmake-out --config Release

Write-Host "Publishing C# WPF app..."
dotnet restore src\csharp\ControllerOptimizer.csproj
dotnet publish src\csharp\ControllerOptimizer.csproj -c Release -r win-x64 -o published --self-contained false

$dll = Join-Path $PSScriptRoot "build\bin\ControllerEngine.dll"
if (-Not (Test-Path $dll)) {
    Write-Error "ControllerEngine.dll not found at $dll"
    exit 1
}

Write-Host "Copying DLL to published folder..."
Copy-Item -Path $dll -Destination (Join-Path $PSScriptRoot "published\ControllerEngine.dll") -Force

$zipPath = Join-Path $PSScriptRoot "ControllerOptimizer-Windows.zip"
if (Test-Path $zipPath) { Remove-Item $zipPath -Force }

Write-Host "Creating ZIP package: $zipPath"
Compress-Archive -Path (Join-Path $PSScriptRoot 'published\*') -DestinationPath $zipPath -Force
Write-Host "Package created: $zipPath"
