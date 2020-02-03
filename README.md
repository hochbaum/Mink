# Mink
Mink is a small and unoptimized x86_64 kernel which I use to learn more about the x86_64 architecture.

## Building
In the `tools/` directory you can find a `CMakeLists.txt` which should fix IntelliSense for your CLion.
If you are using Visual Studio Code, point it to the `.vscode` directory in there so that the includes 
can be resolved.
#### Booting
```make run```
#### Building the ISO
```make```
#### Building the kernel
```make kernel.elf```
