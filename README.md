# Mink
Mink is a small and unoptimized x86_64 kernel which I use to learn more about the x86_64 architecture.

## Building
I have provided a config for Visual Studio Code in case you are using it for C. Don't try building via
CMake. The `CMakeLists.txt` is just here to fix IntelliSense for CLion users.
#### Booting
```make run```
#### Building the ISO
```make```
#### Building the kernel
```make kernel.elf```
