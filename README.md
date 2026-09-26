# Untitled-Graphics-Engine  

My attempt at making a graphics engine using OpenGL. Supports a variety of features, such as geometry shaders, framebuffers and instancing.

A demo scene has been provided which renders the asteroids scene from [learnopengl](https://learnopengl.com/Advanced-OpenGL/Instancing). More demos may be provided to make the engine easier to use.

<img width="600" height="350" alt="Instancing Scene from learnopengl" src="https://github.com/user-attachments/assets/02372564-f114-4cb0-a07e-82cc60621a9a" />

(The asteroids scene, rendering 10,000 asteroids to a window using instancing.)

# To build:
## Linux:

**It is suggested to use ccmake for easier configuration.**
```
mkdir build
cd build
ccmake ..
cmake --build .
```

The executable should build in bin/app

## Windows:
Open Developer Command Prompt for VS 2022.

Navigate to the repo directory e.g.
```cd C:\Users\username\path\to\repo```

Run cmake using the preset release/debug
```
cmake --preset release
cmake --build --preset release
```
The executable should build in bin/app.exe

# Build notes and dependencies:
This project depends on GLFW, gl, glm and assimp.

**Assimp is not included with this repo to keep the size manageable.**
Cmake will automatically try to download and compile assimp if it is not found in the lib folder.
This requires an internet connection.

Alternatively, you may provide your own installation of assimp.so for Linux or assimp-vc143-mtd.dll and assimp-vc143-mtd.lib for Windows. Cmake expects assimp.so and assimp-vc143-mtd.dll to be in the lib folder, and assimp-vc143-mtd.lib to be in the bin folder, if you are using Windows. **Keep in mind that the version used by this repo is 6.0.2**.

# Documenting
This repo has been fully documented using Doxygen. It is highly recommended to read the documentation files in docs/html/index.html before using the engine. If you wish to extend the engine, you may regenerate the Doxygen files using:
```
cd docs
doxygen Doxyfile.in
```
