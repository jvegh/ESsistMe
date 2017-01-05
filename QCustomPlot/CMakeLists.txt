# This is the CMakeLists file generating Qt5-based QCustomPlot library stuff for the ESsistMe tool
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} QCustomPlot library")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
include_directories(
      include
      )

QT5_WRAP_CPP(qcustomplot_moc include/qcustomplot.h)

  # Executables fail to build with Qt 5 in the default configuration
  # without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -Wall")

ADD_LIBRARY(libQCustomPlot
    qcustomplot.cpp
    ${qcustomplot_moc} # The MOC headers, generated
)

set_target_properties(libQCustomPlot
                      PROPERTIES OUTPUT_NAME QCustomPlot
                      )
target_link_libraries(libQCustomPlot Qt5::Widgets)

target_include_directories(libQCustomPlot PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  qcustomplot.cpp
	DESTINATION QCustomPlot
	COMPONENT srcs)

INSTALL(FILES  qcustomplot.h
	DESTINATION QCustomPlot/include
	COMPONENT srcs)


