# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-src"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-build"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/tmp"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src"
  "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/alfahad/Desktop/DSA-GNV-Traffic-Heat-Map/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
