# Brief CMake Tutorial

## What is CMake and Why Use It?

CMake is a build system generator that makes compiling large codebases very easy. You have probably used
Makefiles in your previous coursework to automate the compilation process. Makefiles can get rather
complicated for large projects, however, and they are a pain to maintain. CMake simplifies this by
intelligently generating makefiles for you. CMake can generate platform specific build files as well,
though we will not be using this feature on MAAV since our code runs only on Linux machines. The
generated Makefiles have all the advantages of the Makefiles you would write such as avoiding
recompilation of code whenever possible, but they are easier to maintain and write than Makefiles.

## How to Use CMake

CMake supports in source and out of source builds. This means that it allows you to build your
binaries and libraries in the same directory as the source code or in a different directory. The
latter option is what we are most interested in since it avoids clutter and separates your code for
convenience. The following directory tree is a simplified version of what we will use in MAAV.

	project
	   |
	   |--CMakeLists.txt (top level, project defined here)
	   |
	   |--build (this is where binaries go)
	   |
	   |--cmake (cmake find files, more on this later)
	   |
	   \--src

The *build* directory will contain the binaries build by our build system. The *src* directory will
contain the source code (`.cpp` and `.hpp` files) of our repository. Make sure to keep these
separate. The build directory is _NEVER_ in a git commit or in an online repo (because *build* is in
the `.gitignore`for our repos). The *cmake* directory contains files that CMake uses to find any
external software libraries that we use in our project. More on this later.

CMake is its own language written in a file called *CMakeLists.txt*. This file is generally found at
the top level project directory and each subdirectory. Each *CMakeLists.txt* provides instructions
to CMake on what to do with the files in that directory. CMake has several important variables that
control where things are.

`${PROJECT_SOURCE_DIR}`	This variable describes where the top directory where the `project()` command
			was used.

`${CMAKE_SOURCE_DIR}`	This variable describes where the top level *CMakeLists.txt* file is. This
			is where your project begins. In our case, this is the same as the previous
			variable.

`${CMAKE_CURRENT_SOURCE_DIR}`	This is the directory in which the *CMakeLists.txt* is in. This could
				be any of the *CMakeLists.txt* in the respository.

`${PROJECT_BINARY_DIR}` This is the full path to the top level build directory. This is where you
			tell CMake to generate your Makefiles and build the porject.

`${CMAKE_BINARY_DIR}`	This is another path to the top level directory of the build tree. In our
			case, this is the same as the previous variable.

`${CMAKE_CURRENT_BINARY_DIR}`	The directory in your build directory where the generated files from
				the current *CMakeLists.txt* will go.

### Basic Project Setup

The top level *CMakeLists.txt* must begin by setting the minimum CMake version required using the
`cmake_minimum_required(VERSION <version_number>)` command. The `version_number` we use for MAAV is
2.8. Following this we begin our project using the `project(<name>)` command. This starts your
project and gives it a name.

If you have subdirectories which contain more source files (such as we do in MAAV), you can use the
`add_subdirecotry(dir_name)` command to add that directory. The command will look inside that
directory for a *CMakeLists.txt* telling it how to build the files in that directory.

Now you can add an executable to build. This is done using the
`add_executable(<name> <main> <dependencies>)` command. This command takes the name of the
executable first, then the `.cpp` file where the `main` function is located, then any other `.cpp`
files needed to compile this executable.

If you have multiple executables which have the same dependencies, you should consider making the
dependencies into a library. You can do this using the `add_library(<name> <SHARED|STATIC> <files>)`
command. The first argument is the name of the library you are creating. The second argument is
whether you want a shared or static library. We generally use shared libraries. Any following
arguments describe the files you want to package into a library.

This library can be linked to your executable using the `target_link_libraries(<executable_name>
<libraries>)` command. The first argument is the name of your executable (the same as what you used
in `add_executable()`) and any following arguments are libraries you wish to link to your
executable.

Here is an example:

```
add_library(nature SHARED
	Animal.cpp
	Lion.cpp
	Ant.cpp
	Wombat.cpp
)

add_executable(zoo)

target_link_libraries(zoo
	nature
)
```

C++ Compile flags can be set by modifying the `${CMAKE_CXX_FLAGS}` variable. The order in which any
of these commands are found generally does not matter (though CMake does care at some random times).
Here is an example:

```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
```

The commands can be split into several lines as shown above. Make sure you _append_ the flags to the
variable and not replace the entire contents of the variable. Notice that you need the
`${<var_name>}` syntax when accessing the contents of a variable, but you can simply use the
variable name when telling CMake to assign a value to it using `set()`.

### CMake Find Files

CMake can find several common libraries by itself. This is because it ships with several *Find*
file. These files describe where to find the header files for a particular library. These files are
normally named in the following format: `Find<LibraryName>.cmake`. Some libraries, however, cannot
be found by CMake out of the box. For these, you either have to write your own *Find* file or find
one online. There are several libraries that we use which require their own *Find* files. These
files are found in the *cmake* directory in the above tree. To tell CMake to look here, we must set
the *module path* to this directory using the following command:

`set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")`

The `CMAKE_MODULE_PATH` is another variable that describes where the module *Find* files are
located. We point this to the *cmake* directory by adding the path to the *cmake* directory to
variable containing all module paths. Keep in mind that `CMAKE_MODULE_PATH` can contain more than
one path (for example, it contains the path to the *Find* files that CMake ships with), so make
sure to append your module path, not simply `set` it.

After you have told CMake *how* to find libraries, you must ask it to actually find those libraries.
This is done using the `find_package(<LibraryName> <REQUIRED?>)`. The `LibraryName` is the same as
the `LibraryName` in the find file described above. The second option allows you to choose whether
that library is required for this project. For our case, they normally will be required. Once CMake
finds the library, it will define several variables for you to use. The two important ones are
`${<LibraryName>_INCLUDE_DIRS}` and `${<LibraryName>_LIBRARIES}` or `${<LibraryName>_LIBRARY}`.
You will use these variables when telling CMake how to link your project.

Now you can then use the `include_directories()` command to include the directories for the header
files of the external project to your current project. This makes it so that you do not have to type
in the full path to the header files for external libraries. You can also use
`target_link_libraries()` to link these external libraries to your project.

Here is a simple example:

```
find_package(ZCM REQUIRED)
include_directories(${ZCM_INCLUDE_DIRS})
add_executable(my-executable
	my-executable.cpp
	Dependency.cpp
	SomeOtherDependency.cpp
)

target_link_libraries(my-executable
	${ZCM_LIBRARY}
)
 ```

## Building the project

To build the project, you must go to your *build* directory and execute the following commands:

```
cmake ..
make -j
```

The `cmake ..` command generates Makefiles from the *CMakeLists.txt* in the parent directory. The
`make -j` command uses the generated Makefiles to build the project. The `-j` flag allows `make` to
compile in parallel using all of the cores on your machine. This is generally much faster than a
regular make command, but it is not necessary. You can specify the number of cores you want `make`
to use by adding a number after the `-j`. For example `-j4` will use 4 cores.

## Time to Practice

To get some practice with CMake, you a copy of MAAV's project 1 is located here without any of the
CMake files. It is your job get this to compile. We have given you all the Find files you need in
the *cmake* directory. The only external libraries you need to link are *GTKMM*m *GLM*, and *Epoxy*.
There is only one executable so you should link everything to it. There is also an install script in
the *scripts* directory to install these external libraries for you. To execute it, just type
`./install-deps.sh` into your terminal from the *scripts* directory and enter your sudo password.

Note that *Epoxy* and *GTKMM* use their own system (namely `pkg-config`)for finding their files.

Instead of
```
find_package(EPOXY REQUIRED)
find_package(GTKMM REQUIRED)
```
you should do
```
include(FindPkgConfig REQUIRED)
pkg_check_modules(EPOXY REQUIRED epoxy)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
```

This will have the same effect as if you had done `find_package()`. You can link *GLM* in the
original way described before this using `find_package()`.

This covers only that basics of CMake. You can do much, much more with it. You can learn more by
doing any of the following:

* Looking through the MAAV repo for *CMakeLists.tst*
* Practicing on your own projects (or on MAAV projects)
* Looking at documentation here: https://devdocs.io/cmake~3.9/
* CMake tutorial and documentation on their website:
	* Tutorial: https://cmake.org/cmake-tutorial/
	* Documentation: https://cmake.org/documentation/
