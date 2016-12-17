##############################################################
#  
#  This is a CMake configuration file for the ESsistMe project.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
# The directory structure (and other docs) can be found in cmake/Docs
##############################################################
#
# @author János Végh
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

cmake_policy(SET CMP0048 NEW)
# Set the name of your project here
set(CPACK_PACKAGE_NAME "essistme")
project(ESsistMe VERSION 0.0.03 LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.0)
###############################################################
# Do not edit below this line until you knpw what you are doing! 
###############################################################
#  
# ---[ Set up CMake options for ESsistMe
if(BUILD_TESTS)
  option (BUILD_MINUIT_TESTS "Include Minuit math and unit tests"  OFF)
endif(BUILD_TESTS)
# Choose whether the stand-alone unit testing is to be built
option (BUILD_TESTS "Include unit tests for the tool"  ON)
# Choose whether to make documention as well
option (BUILD_DOCS "Prepare also HTML/CHM documentation" ON)
# Choose whether to use asserts
option (NDEBUG "Allow to assert the simulator" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
# Choose whether you need debugging support for development or making a release 
option (DEBUG_MODE "Include debug support for the program"  ON)


include(cmake/Colorize.cmake)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_STANDARD 11)

# Set the build type if it isn't already
if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_WORK_DIRECTORY "${CMAKE_BINARY_DIR}")
  message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION} debug version")
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_WORK_DIRECTORY "${CMAKE_BINARY_DIR}")
  message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION} release version")
endif(DEBUG_MODE)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CPACK_PACKAGE_NAME}")
#set(CMAKE_PREFIX_PATH "/home/jvegh/Qt/5.5")

# ---[ The out-of-source build directory
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# ---[ Using cmake scripts and modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/External)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(ExternalProject)
include(cmake/Summary.cmake)
include(cmake/Utils.cmake)

# ---[ Dependencies 
include(cmake/Dependencies.cmake)

# ---[ Flags
if(UNIX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall")
endif()

if(USE_libstdcpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  message("-- Warning: forcing libstdc++ (controlled by USE_libstdcpp option in cmake)")
endif()

# Copy the version and other configuration info to the source
configure_file(${CMAKE_SOURCE_DIR}/main/Config.h.in ${CMAKE_SOURCE_DIR}/main/include/Config.h)
add_subdirectory(Minuit) # The Minuit minimization library
add_subdirectory(SPeval) # The SPeval spectrum evaluation library
add_subdirectory(QtGUI)	# The Qt-based graphics routines
add_subdirectory(main)	# The main executables of the ES tool gui
if (BUILD_TESTS)
  add_subdirectory(test)	# Tests for the tool library
  add_subdirectory(test/Minuit)	# Tests for the Minuit library
endif (BUILD_TESTS)
if(BUILD_DOCS)
  set(doxyfile_in ${CMAKE_SOURCE_DIR}/doc/documentation-config.doxygen.in)
  set(doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
  add_subdirectory(doc) # The doxygen documentation
endif(BUILD_DOCS)

# ---[ Target 'package' builds everything
include(cmake/Packages.cmake)

# ---[ Print configuration summary
caffe_print_configuration_summary()
# https://wiki.qt.io/Install_Qt_5_on_Ubuntu
