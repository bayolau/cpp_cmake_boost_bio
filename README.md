# cpp_cmake_boost_bio
self-contained c++/cmake/boost infrastructure for prototyping bioinformatics ideas.

The point is for one to quickly add a private/public GIT submodule into ```src/modules``` for rapid development.

```src/modules``` - each sub-direcotry is compiled into a module, dispatch code is automatically generated for ```bare```. see sandbox for example.

```src/bio``` - reusable bioinformatic code structure, mostly for quick prototyping

```opt``` - external code/include/lib

```git clone --recursive <URL>``` will clone all submodules, including Boost (395M) and SeqAn (257M). Such download can be skipped if you know what you're doing: clone without ```--recursive``` and improvise.
