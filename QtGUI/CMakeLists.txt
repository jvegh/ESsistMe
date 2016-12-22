# This is the CMakeLists file generating Qt5-related stuff for the ESsistMe tool
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} GUI library with Qt5")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS
      Core Widgets Concurrent PrintSupport LinguistTools REQUIRED)

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
include_directories(
      include
      ${CMAKE_SOURCE_DIR}/QCustomPlot/include
      ${CMAKE_SOURCE_DIR}/main/include
      ${CMAKE_SOURCE_DIR}/Minuit/include
      ${Qt5Widgets_INCLUDE_DIRS}
      ${Qt5Printer_INCLUDE_DIRS}
      ${Qt5Concurrent_INCLUDE_DIRS}
      )

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Printer_DEFINITIONS} ${Qt5Concurrent_DEFINITIONS})

set(QOBJECT_HEADERS 
	include/ENavigatorWindow.h
	include/EResidualWindow.h
	include/ESpectrumWindow.h
	include/ESsistMe_mainwindow.h
	include/FitDialog.h
	include/QStuff.h
)

set(QOBJECT_SOURCES 
	ENavigatorWindow.cpp
	EResidualWindow.cpp
	ESpectrumWindow.cpp
	ESsistMe_mainwindow.cpp
        FitDialog.cpp
	QStuff.cpp
)


file(GLOB_RECURSE QOBJECT_IMAGES
    "images/*.png"
)

file(GLOB_RECURSE QOBJECT_UIS
    "include/*.ui"
)

QT5_WRAP_CPP(hdr_moc ${QOBJECT_HEADERS})
qt5_add_resources(mainresource essistme.qrc)

  # Executables fail to build with Qt 5 in the default configuration
  # without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}  -std=c++11 -Wall")


ADD_LIBRARY(libQtGUI
    ${QOBJECT_SOURCES}
	${hdr_moc} # The MOC headers, generated
	${mainresource}	# The resource file of the main window
)

# Use the Widgets module from Qt 5.
target_link_libraries(libQtGUI  libQCustomPlot
Qt5::Widgets Qt5::PrintSupport Qt5::Concurrent)

set_target_properties(libQtGUI 
                      PROPERTIES OUTPUT_NAME QtGUI
                      )

target_include_directories(libQtGUI PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(libQCustomPlot PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  ${QOBJECT_SOURCES} application.qrc CMakeLists.txt
	DESTINATION QtGUI
	COMPONENT srcs)

INSTALL(FILES  ${QOBJECT_HEADERS}  ${QOBJECT_UIS}
	DESTINATION QtGUI/include
	COMPONENT srcs)

INSTALL(FILES  ${QOBJECT_IMAGES}  images/ES_logo.png
	DESTINATION QtGUI/images
	COMPONENT srcs)

