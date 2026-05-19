# Rocket League Controller Optimization System

This workspace contains a Windows-focused controller optimization project for Rocket League. It includes a C++ engine for HID input processing, polling rate optimization, and anti-drift compensation with a C# WPF frontend.

## Build & Download ZIP (automated)

A GitHub Actions workflow `Windows Build and Package` is included and will build the native DLL and the C# WPF app on `windows-latest`, then produce a ZIP artifact named `ControllerOptimizer-Windows.zip`.

To trigger a build and download the ZIP:

- Push to the `main` branch (workflow runs on push), or run the workflow manually from the Actions tab (`Windows Build and Package`).
- After the workflow completes, go to the workflow run -> `Artifacts` -> download `ControllerOptimizer-Windows` ZIP.

Alternatively, build locally on Windows using the included PowerShell script:

```powershell
# From repo root on Windows PowerShell (with Visual Studio & .NET SDK installed)
scripts\build-and-zip.ps1
# Output: ControllerOptimizer-Windows.zip (and published/ folder)
```

To create a source-only ZIP (Linux/macOS):

```bash
scripts/create-source-zip.sh
# Output: rocketleague-source.zip
```

## Notes

- The build workflow requires Windows runners; artifacts are downloadable from GitHub Actions.
- If you want a GitHub Release automatically created with the ZIP attached, I can add that next.
