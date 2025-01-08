# SDF-Engine

## Create Windows Build for Visual Studio 17 2022

```sh
mkdir windows_build && cd windows_build && cmake -G "Visual Studio 17 2022" -A x64 ..
```

After running this command, u can see .sln file ( Visual Studio solution ). Build ```sdfe``` from this file.

## Create MacOS Build for Xcode
```sh
mkdir macos_build && cd macos_build && cmake -G Xcode ..
```

## Linux???
```sh
mkdir build && cmake .. && make
```

Done.

