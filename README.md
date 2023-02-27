# Arklumos engine

Arklumos game engine

# Before starting
- Clone the repo and check the submodules (spdlog, GLFW, ImGui)
- Check the folder acmake-required to copy the CMakeLists.txt to the folder Arklumos/vendor/imgui (required for CMake to process ImGui library)

## For Visual Studio Code
You need to copy (depending on your OS):
- linux--c_cpp_properties.json.example
- windows--c_cpp_properties.json.example

To c_cpp_properties.json in the folder .vscode

Pay attention to these lines, you might need to changes them:
"compilerPath": "C:\\Devc++\\mingw64\\bin\\g++.exe",
"intelliSenseMode": "windows-gcc-x64",



# For cmake alone (needs to be installed)
Check the CMakeLists.txt for all dependencies if anything needs to be changed
run: cmake --build build --config Release
# For cmake plus ninja (need ninja - choco install ninja for Windows)
## Ninja for faster compilation of the project
Check the CMakeLists.txt for all dependencies if anything needs to be changed
run first: $ cmake -B build -G Ninja
then: $ cd build
after, it's only: $ ninja //(-j 10 = depending on how many jobs in parallel, 10 here)

https://ilyas-hamadouche.medium.com/build-your-c-projects-faster-using-ninja-3d7af9b418fc
https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages


# For premake windows
# Execute the build.bat
# To generate the bin and bin-int files
$ make                # build default (Debug) configuration
$ make config=release # build release configuration
$ make help           # show available configurations

NOTA: if make config=release is stuck on first launch, quit it and re-execute the command (The folders inside bin might not create properly)