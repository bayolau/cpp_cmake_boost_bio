# cpp_cmake_boost_bio
self-contained c++/cmake/boost infrastructure for bioinformatics projects. ```git clone --recursive <URL>``` will clone all submodules, including Boost (395M) and SeqAn (257M). Such download can be skipped if you know what you're doing: clone without ```--recursive``` and improvise.

src - source directory

src/modules - module dispatch

src/modules/name - new modules of "name" should go here, see sandbox

opt - stores 3rd party code

opt/include - 3rd party includes

opt/lib - 3rd-party libraries

