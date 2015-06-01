#----------------------------------------------------------------------------
# Compile command, from inside build directory:
# cmake ../
# make -j4

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(RawData)
#include_directories(/home/mage/DataAnalysis-QuickStart/CalTech)
add_compile_options(-std=c++11)

#----------------------------------------------------------------------------
#For Mac OS X Use @rpath on OS X
if(APPLE) 
  if(POLICY CMP0042)
      CMAKE_POLICY(SET CMP0042 NEW)
   endif()
endif()

#-------------------------------
# ROOT dependencies
#-------------------------------

######## locate ROOT install, headers, includes
include(FindROOT.cmake)
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
#################
# Executables
#################


add_library(RawData_static STATIC RawData.cpp)
add_library(RawData SHARED RawData.cpp)
target_link_libraries(RawData ${ROOT_LIBRARIES})
#target_link_libraries(main RawData)
add_library(PmtData SHARED RawData.cpp)
add_library(PmtData_static STATIC PmtData.cpp)
target_link_libraries(PmtData ${ROOT_LIBRARIES})
#Uncomment this next two lines if you want and example program buit with your code

#add_executable(example example.cpp) 
#target_link_libraries(example ${ROOT_LIBRARIES} RawData)


#################
# Build settings
#################

#set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -Wextra -Wpedantic")
