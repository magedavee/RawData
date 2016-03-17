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

set(RAW "${PROJECT_SOURCE_DIR}/src/RawData.cpp")
set(RAWH "${PROJECT_SOURCE_DIR}/include/RawData.h")

set(PMT "${PROJECT_SOURCE_DIR}/src/PmtData.cpp")
set(PMTH "${PROJECT_SOURCE_DIR}/include/PmtData.h")
#-------------------------------
# ROOT dependencies
#-------------------------------

######## locate ROOT install, headers, includes
include(FindROOT.cmake)
include_directories(${ROOT_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/include/")
link_directories(${ROOT_LIBRARY_DIR})
#################
# Executables
#################

set(SB_G4_LINKDEF ${RAWH})
set(SB_G4_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/RawDataLib_Dict.cxx)
MESSAGE("Generating dictionary ${SB_G4_DICTIONARY}")
set(DICTFILES ${PROJECT_SOURCE_DIR}/include/RawData.h)
MESSAGE( STATUS "DIC " ${DICTFILES})
ROOT_GENERATE_DICTIONARY("${DICTFILES}" "${SB_G4_LINKDEF}" "${SB_G4_DICTIONARY}" "")
LIST(APPEND RAW ${SB_G4_DICTIONARY})

set(SB_G4_LINKDEF ${PMTH})
set(SB_G4_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/PmtDataLib_Dict.cxx)
MESSAGE("Generating dictionary ${SB_G4_DICTIONARY}")
set(DICTFILES ${PROJECT_SOURCE_DIR}/include/PmtData.h)
MESSAGE( STATUS "DIC " ${DICTFILES})
ROOT_GENERATE_DICTIONARY("${DICTFILES}" "${SB_G4_LINKDEF}" "${SB_G4_DICTIONARY}" "")
LIST(APPEND PMT ${SB_G4_DICTIONARY})

add_library(RawData SHARED ${RAW} ${RAWH})
target_link_libraries(RawData ${ROOT_LIBRARIES})

add_library(PmtData SHARED ${PMT} ${PMTH})
target_link_libraries(PmtData ${ROOT_LIBRARIES})

#################
# Build settings
#################

#set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -Wextra -Wpedantic")
