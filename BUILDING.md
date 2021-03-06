

<!-- toc -->

- [Getting started for development](#getting-started-for-development)
  * [Prerequisites](#prerequisites)
    + [Linux](#linux)
    + [MacOS](#macos)
    + [Windows](#windows)
  * [Get the code](#get-the-code)
  * [Building](#building)
  * [Running unit tests](#running-unit-tests)
    + [Coverage information](#coverage-information)
- [CMake Configuration Options](#cmake-configuration-options)
  * [GILES_BUILD_DOCUMENTATION](#giles_build_documentation)
  * [GILES_CALCULATE_COVERAGE](#giles_calculate_coverage)
- [Packaging](#packaging)

<!-- tocstop -->

## Getting started for development

### Prerequisites

This uses [CMake.](https://cmake.org/) As such, CMake needs to be
installed first.
Additionally, [Boost](https://www.boost.org/) needs to be installed manually.

#### Linux

The recommended way of installing CMake and Boost is through your package
manager.

If either package isn't available on your Linux distribution follow the
[official CMake install instructions](https://cmake.org/install/)
or the [official boost instructions.](https://www.boost.org/users/download/)

#### MacOS

CMake and boost can be installed using [Homebrew](https://brew.sh/) with the
following command.
```
brew install cmake boost-python
```
Alternatively, follow the
[official CMake install instructions](https://cmake.org/install/)
and the [official boost instructions.](https://www.boost.org/users/download/)

#### Windows

Follow the [official CMake install instructions](https://cmake.org/install/)
and the [official boost instructions.](https://www.boost.org/users/download/)

### Get the code

It is highly recommended, but not essential, to use git to get the code.
**Use this command** to get a copy:
```
git clone --recurse-submodules https://github.com/bristol-sca/GILES
```
**Note that the
[--recurse-submodules flag](https://www.git-scm.com/docs/git-clone#Documentation/git-clone.txt---recurse-submodulesltpathspec)
is needed.**

If downloading manually or without this flag, you will also need to download the 
projects
[in the external directory.](https://github.com/bristol-sca/GILES/tree/master/external)

Additionally the
[--jobs flag](https://www.git-scm.com/docs/git-clone#Documentation/git-clone.txt---jobsltngt)
can be used to speed this process up.

### Building

1) **Create an empty build directory.**

2) **From the build directory you just created, run this command.**
This will generate native build files for your platform. 
A specific generator can be specified using the
`-G *generator*` flag.
A [list of generators is available here.](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
```
cmake /path-to-source-directory
```

3) **Run this command to compile the code.** This will compile the program using
your chosen generator e.g. make. Alternatively the native build system can be
used manually from this point on.
```
cmake --build .
```

### Running unit tests

1) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

2) **Build the tests with this command.** This will tell the native build system
to build the target called GILES-tests.
```
cmake --build . --target GILES-tests
```
3) **Run the tests**
```
/path-to-build-directory/bin/tests
```

#### Coverage information

In order to generate code coverage information, [Gcovr](https://gcovr.com/) is
required.

1) **Regenerate the build files** using the option [shown below](#GILES_calculate_coverage) from the build
directory. This is needed as specific compile flags need to be added in order to
generate coverage information.
```
cmake /path-to-source-directory -DGILES_CALCULATE_COVERAGE=ON
```

2) **Run this command.** This will tell the native build system to build the
target called coverage.
```
cmake --build . --target coverage
```
Coverage information can be found by opening the file `coverage.html` in the
folder `coverage` which should have been created in your *`build directory`*.
```
/path-to-build-directory/coverage/coverage.html
```

## CMake Configuration Options

These are all CMake options and can be appended to the CMake generate command.
For example, from the build directory:
```
cmake /path-to-source-directory -DGILES_BUILD_DOCUMENTATION
```

### GILES_BUILD_DOCUMENTATION

This option will always built the "doc" target when the target "all" is built.
See [API Documentation section.](#api-documentation)

### GILES_CALCULATE_COVERAGE

This is needed to generate code coverage information.
See [Coverage information](#coverage-information) for details on how to do this.

**⚠️ Warning ⚠️
This will enable compile flags that are not recommended for normal
purposes. Set this to OFF after you are done generating coverage information.**

## Packaging

CMake's CPack is used for packaging.

The command to build and package for your current operating system is
```
cmake --build . --target package --config release
```

Packaging as an [RPM](https://en.wikipedia.org/wiki/RPM_Package_Manager)
package requires rpmbuild to be installed.
