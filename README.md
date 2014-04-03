## Build

### CMake

- Finding 3rd party libraries on different platforms
- QtCreator supports CMake project
- Tons of C++ projects are using it, including OGRE and Bullet
- Out-of-source build keeps source directory clean

### With QtCreator

1. Open QtCreator
2. Open File or Project
3. Open ./CMakeLists.txt
4. Choose a build folder, it can be anywhere as long as not the project's root directory. Be sure not to commit this folder to git.
5. Hit "Run CMake" and "Done"
6. Build and debug as usual. The output binary is at ./game/Linux/Debug

CMake must be run again after adding/deleting/renaming a file, or the changes won't be reflected in QtCreator.

### Without QtCreator

```
cd velonaut
mkdir build
cd build
cmake ..
make
```

CMake must be run again after adding/deleting/renaming a file.

## Directory Structure
```
./game
	Game assets, and binaries for each platforms and build configurations
./lib
	Precompiled 3rd party libs and headers
./cmake
	CMake files for finding libraries
./src/engine
	Engine source code, now compiled to a static library
./src/game
	Game source code, now compiled to a binary at ./game. 
	In the future this directory can be removed if the engine becomes fully data-driven
./asset
	Can be used to store raw game assets, like Maya files, in the future.

```
