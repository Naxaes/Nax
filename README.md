# Nax
Nax is a simple render engine. It probably stands for something amazingly clever, and is most likely not an abbreviation of my username.

#### Master [![Build Status](https://travis-ci.org/Naxaes/Nax.svg?branch=master)](https://travis-ci.org/Naxaes/Nax)
#### Development [![Build Status](https://travis-ci.org/Naxaes/Nax.svg?branch=development)](https://travis-ci.org/Naxaes/Nax)


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

To contribute, you need to follow the proper workflow and conventions.

### Workflow
#### 1. Create an issue
Before starting anything, an issue **must** be created. This allows for discussion whether it's relevant and how it should be implemented. This can be done if you want to implement something yourself, or just want to something to be implemented.
  1. Create an issue with a short summative title with appropriate tags (at least one is needed).
  2. Start a discussion with fellow peers and contributors.
  3. When a contributor has given affirmation, assign the issue to the one(s) implementing the change.

#### 2. Create a branch
When an issue has been created, discussed and assigned, the assignee can create a new branch. The branch **must** be named
```<most-appropriate-tag>-<issue-number>/<description>``` 

Some examples are:
```
feature-142/notification-system
bug-15/division-by-zero
documentation-244/shader-test
```

#### 3. Create a pull request
After the change has been implemented in the local branch, it should be pushed to the remote branch (with the same name). Thereafter, a pull request **must** be created before merging it to any other branch. This allows for discussion whether it's implemented correctly and follow the conventions.
  1. Create an issue with a short summative title and the line "Fixes #\<issue-number\>" at the first line in the description.
  2. Write what has been changed and how correctness has been confirmed. Also write if something hasn't been cofirmed that could be a potential problem, for example "Didn't test on Windows" or "Didn't check with files over 2 GB".
  3. Start a discussion with fellow peers and contributors.
  4. When a contributor has reviewed and given affirmation, the branch is applicable for merging and deletion.
    
### Coding convention
The code should follow [Google C++ style Guide](https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes).


## Versioning

The project uses [Semantic versioning](http://semver.org/).

## Authors

* **Ted Klein Bergman** - Creator.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Nothing here yet.

