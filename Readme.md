# LearnOpenGL

Example from course : https://learnopengl.com/

## Requirement

* C++20 Compatible compiler
* CMake 3.14

## Build & Run

You only need a compiler and cmake 3.14 minimum. CMake will fetch all dependencies using `FetchContent`. All the dependencies are downloaded from github, build and statically linked so you don't need to install any library before.

```bash
mkdir build && cd build
cmake ..
make -j
```
