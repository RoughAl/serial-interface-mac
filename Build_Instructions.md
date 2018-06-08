# Serial Interface MAC Build Instructions
```
     ___ _____ _   ___ _  _____ ___  ___  ___ ___
    / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
    \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
    |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
    embedded.connectivity.solutions.==============
```

# Build Instructions

## Building the Library
The STACKFORCE Serial MAC uses CMake as build system. Note that, once built, the library can be used by other cmake projects with the `find_package()` command without requiring to install the library on the system. Therefore running a `make install` is not mandatory to be able to build other projects that link to the serialmac library.

Go to root, create build directory:

    cd serial-interface-mac
    mkdir build

and run:

    cmake ..
    make
    sudo make install

or to define a custom install directory e.g. devroot:

    cmake -DCMAKE_INSTALL_PREFIX=devroot ..
    make
    make install

To generate packages run:

    make package

This will generate a tar.gz archive, and installer shell script by default.
If run under Ubuntu, Debian or LinuxMint, a **deb** package will be generated.
Use **dpkg** as follows to install the package.

    dpkg -i package_name.deb

## Building the Documentation
To generate the doxygen documentation run:

    cmake -DBUILD_DOC=on ..
    make doc

Doxygen documentation will generated under `<build_directory>/doc/html`. Open the **index.html** file within that directory with a web browser to access the generated documentation.

## Building the Unit Tests

To build the library together with the unit tests use the **BUILD_TESTING** cmake option as follows:

    cmake -DBUILD_TESTING=on ..
    make

To run the unit tests run `make test` within cmake's build directory.

_________________________________

[Serial Interface MAC](README.md)