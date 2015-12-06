#Copyright (c) 2015, Bayo Lau
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#* Redistributions of source code must retain the above copyright notice, this
#list of conditions and the following disclaimer.
#
#* Redistributions in binary form must reproduce the above copyright notice,
#this list of conditions and the following disclaimer in the documentation
#and/or other materials provided with the distribution.
#
#* Neither the name of cpp_cmake_boost_bio nor the names of its
#contributors may be used to endorse or promote products derived from
#this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

add_library(modules ${CMAKE_CURRENT_LIST_DIR}/dispatch.cpp)
if(TARGET boost)
  add_dependencies(modules boost)
endif(TARGET boost)
set (EXTRA_LIBS ${EXTRA_LIBS} modules)

include(${SRC_DIR}/cmake/sub_dir_list.cmake)
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

#automatically discover modules/<name>/main.cpp and compile into a library named <name>
set( MODLUE_LIST )
set( INCLUDE_LIST )
foreach (sub ${SUBDIRS})
  message(STATUS "adding module ${sub}")
  CC_LIST(cpp ${CMAKE_CURRENT_LIST_DIR}/${sub})
  add_library(${sub} ${cpp})
  add_dependencies(${sub} buildinfo)
  if(TARGET boost)
    add_dependencies(${sub} boost)
  endif(TARGET boost)
  set (INCLUDE_LIST "${INCLUDE_LIST}#include \"modules/${sub}/main.h\"\n")
  set (MODULE_LIST "${MODULE_LIST}(${sub})")
  set (EXTRA_LIBS ${EXTRA_LIBS} ${sub})
endforeach (sub ${SUBDIRS})

message(STATUS ${MODULE_LIST})
configure_file ( "${SRC_DIR}/modules/module_list.h.in" "${PROJECT_BINARY_DIR}/module_list.h" )
