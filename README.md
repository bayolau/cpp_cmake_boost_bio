# cpp_cmake_boost_bio
self-contained c++/cmake/boost infrastructure for prototyping bioinformatics ideas.

The point is for one to quickly add a private/public GIT submodule into ```src/modules``` for rapid development.

```src/modules``` - each sub-direcotry is compiled into a module, dispatch code is automatically generated for ```bare```. see sandbox for example.

```src/bio``` - reusable bioinformatic code structure, mostly for quick prototyping

```opt``` - external code/include/lib

```git clone --recursive <URL>``` will clone all submodules. If you know what you're doing, improvise and skip such download, and turn off Boost/HDF5 build in CMakeLists.txt.
