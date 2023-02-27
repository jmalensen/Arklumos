# Arklumos engine

Arklumos game engine

# For cmake alone (needs to be installed)
Check the CMakeLists.txt for all dependencies if anything needs to be changed
run: cmake --build build --config Release
# For cmake plus ninja (need ninja - choco install ninja for Windows)
## Ninja for faster compilation of the project
Check the CMakeLists.txt for all dependencies if anything needs to be changed
run first: $ cmake -G Ninja
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