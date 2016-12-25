# This is the main CMakeLists file of the ESsistMe project
# It generates the ESsistMe main executable program GUI version
#
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

set(CMAKE_CXX_STANDARD 11)

set( MY_SRCS
   Stuff.cpp
)
message(HIGHLIGHTED "Configuring ${PROJECT_NAME} GUI tool")

## Deal with the GUI version
find_package(Qt5Widgets)
find_package(Qt5 COMPONENTS
      Core Widgets Concurrent PrintSupport LinguistTools REQUIRED)
# The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES}
        ${Qt5Concurrent_INCLUDES}
        ${Qt5Printer_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
        include
        ${CMAKE_SOURCE_DIR}/SPeval/include 
        ${CMAKE_SOURCE_DIR}/QtGUI/include
	${CMAKE_SOURCE_DIR}/Minuit/include
        ${CMAKE_SOURCE_DIR}/QCustomPlot/include

)

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

ADD_LIBRARY(libMain
	${MY_SRCS}
	${mainresource}	# The resource file of the main window
)

set(CMAKE_CXX_FLAGS )
# Tell CMake to create the  executable
add_executable(${PROJECT_NAME}
	ESsistMeMain.cpp
	)
# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME}
	libMinuit libQCustomPlot
        libQtGUI libMain libSPeval
	Qt5::Widgets Qt5::Concurrent Qt5::PrintSupport
	${Caffe_LINKER_LIBS}
	${Pthread}
)

install(TARGETS ${PROJECT_NAME}
   RUNTIME
   DESTINATION bin
   COMPONENT apps)


INSTALL(FILES ${MY_SRCS} ESsistMeMain.cpp Config.h.in CMakeLists.txt
	DESTINATION main
	COMPONENT srcs)

INSTALL(FILES include/Stuff.h
	DESTINATION main/include
	COMPONENT srcs)


## http://stackoverflow.com/questions/3702115/creating-a-directory-in-cmake
	
INSTALL(FILES  	../cmake/Colorize.cmake 
		../cmake/Dependencies.cmake
		../cmake/Packages.cmake
		../cmake/Summary.cmake
		../cmake/Targets.cmake
		../cmake/Utils.cmake
	DESTINATION cmake
	COMPONENT srcs)
	
INSTALL(FILES ../cmake/External/gtest.cmake 
	DESTINATION cmake/External
	COMPONENT srcs)

INSTALL(FILES   ../cmake/Modules/FindGtest.cmake
	DESTINATION cmake/Modules
	COMPONENT srcs)



