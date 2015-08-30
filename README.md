# cpp_cmake_boost_bio
self-contained c++/cmake/boost infrastructure for prototyping bioinformatics ideas.

One can quickly add a private/public GIT submodule into ```src/modules``` for rapid development.

Yes, I have private code -- anything interesting related to my employer.

```src/modules``` - each sub-direcotry is compiled into a module, dispatch code is automatically generated for ```bare```. see sandbox for example.

```src/bio``` - reusable bioinformatic code structure, mostly for quick prototyping

```opt``` - external code/include/lib

Tested for:

```CMake``` >= ```3.2.2``` (```<= 2.8.9``` is known to fail)

```Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)```

```gcc >= 5.10``` (```<= 4.8.2``` has known bugs / missing c++11 features)

Instructions:
```git clone --recursive <URL>``` will clone all submodules. If you know what you're doing, improvise and skip such download, and turn off Boost/HDF5 build in CMakeLists.txt.
