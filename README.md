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
4. Hit "Run CMake" and "Done"
5. Build and debug as usual

CMake must be run again after adding/deleting/renaming a file, or the changes won't be reflected in QtCreator.

### Without QtCreator

```
cd velonaut
mkdir build
cd build
cmake ..
make
../game/velonaut
```

CMake must be run again after adding/deleting/renaming a file.

## Directory Structure
```
./game
	Game assets and binary
./lib
	Precompiled 3rd party libs
./cmake
	CMake files for finding libraries
./src/engine
	Engine source code, now compiled to a static library
./src/game
	Game source code, now compiled to a binary at ./game. 
	In the future this directory can be removed if the engine is fully data-driven
./asset
	Can be used to store raw game assets in the future.

```
