# FS-reader

## Team

 - [Babin Igor](https://github.com/igor185)

## Prerequisites

 - **C++ compiler** - needs to support **C++17** standard
 - **CMake** 3.15+
 
Dependencies (such as development libraries) can be found in the [dependencies folder](./dependencies) in the form of the text files with package names for different package managers.

## Installing

1. Install required packages.

   On Ubuntu:
   ```bash
   [[ -r dependencies/apt.txt ]] && sed 's/#.*//' dependencies/apt.txt | xargs sudo apt-get install -y
   ```
   On MacOS:
   ```bash
   [[ -r dependencies/homebrew.txt ]] && sed 's/#.*//' dependencies/homebrew.txt | xargs brew install
   ```
   Use Conan on Windows.
2. Build.
    ```bash
    cmake -Bbuild
    cmake --build build
    ```

## Usage

```bash
./reader --type EXT --path ../data/ext2.img
```

Help flags `-h`/`--help` support is available.
