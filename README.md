# PvZ-Raylib

A Plants vs. Zombies clone written in C++ using [Raylib](<https://github.com/raysan5/raylib>)

## Note

This is a project I'm making while relearn more C++
The game is partially functional, but has no real gameplay yet

## Downloads

You can use GitHub Actions to download the binary files on your favorite desktop os

## Build from source

> Make sure to have CMake v3.15+ installed

1. Clone the repo

    ```bash
    $ git clone --recurse-submodules https://github.com/thqnhz/PvZ-Raylib
    $ cd PvZ-Raylib
    ```

    > If you have already cloned the repo, run this command to update the submodules
    >
    > ```bash
    > $ git submodule update --init --recursive
    > ```

2. Create the build directory and cd into it

    ```bash
    $ mkdir build
    $ cd build
    ```

3. Use CMake to generate the build file and build the game

    ```bash
    $ cmake ..
    $ cmake --build .
    ```
