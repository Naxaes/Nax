# Nax
Nax is a simple render engine. It probably stands for something amazingly clever, and is most likely not an abbreviation of my username.

## Getting Started

To copy the project, simply press the button *Clone or download*. Press *Download zip* for to download the project, or copy the link to clone the project using git.
```
git clone https://github.com/Naxaes/Nax.git
```

### Prerequisites

The project requires CMake (version 3.2 or higher), and C++ (14 or higher).

### Installing

IDE's as Clion and Visual Studio should be able to compile the project by simply hitting "build". Building manually, using the command line, is however possible using these steps:

**1. Create a build directory.**

Cmake will generate a lot of helper files, which could clutter up your workspace. It's recommended to create a folder for these (although not necessary). Navigate to your project folder and run following command:

```
mkdir build
```

It will generate a folder called build.

**2. Run CMake.**

Navigate to the build folder and run CMake. CMake requires the CMakeLists.txt file in order to generate a make file. If you're in the build folder, the CMakeLists.txt file should be the directory above. You can pass in the relative path to CMake.

```
cd build
cmake ../
```

**3. Run Make.**

CMake should now have created a folder with a bunch of files. Now is the time to run make. Make sure you're still in the build directory.

```
make
```

**4. Run the program.**

Make will generate all executables in the project.

```
./Nax
```

Or any other name of the executable you want to be executed.


## Running the tests

If you've successfully compiled the project, then running the tests are as simple as run the corresponding executable.

```
./test
```

## Deployment

Currently not available.

## Built With

* [glm](https://github.com/g-truc/glm) - For linear algebra.
* [assimp](https://github.com/assimp/assimp) - For model loading.
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) - For loading images and textures.
* [imgui](https://github.com/ocornut/imgui) - For quickly and easily setting up widgets for debugging purposes.
* [glfw](https://github.com/glfw/glfw) - For window creation.
* [gtest](https://github.com/google/googletest) - For unit testing.
* [glad](https://glad.dav1d.de/) - For binding OpenGL bindings.

## Contributing

Not available yet.

## Versioning

The project uses [Semantic versioning](http://semver.org/).

## Authors

* **Ted Klein Bergman** - Creator.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Nothing here yet.

