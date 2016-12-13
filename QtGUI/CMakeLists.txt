# This is the CMakeLists file generating Qt5-related stuff for the ESsistMe toolt
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} Qt5 GUI")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5PrintSupport REQUIRED)
#find_package(Qt5 COMPONENTS Core Network 
##Svg WebKit WebKitWidgets 
#Widgets Xml 
##XmlPatterns 
#Concurrent PrintSupport LinguistTools)
#find_package(Qt5 COMPONENTS Core Network WebKit WebKitWidgets Widgets Concurrent PrintSupport)


  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES} ${Qt5Printer_INCLUDES}
		include
		${CMAKE_SOURCE_DIR}/main/include 
		${CMAKE_SOURCE_DIR}/Minuit/include
		)

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Printer_DEFINITIONS})

file(GLOB_RECURSE QOBJECT_HEADERS
    "include/*.h"
)

file(GLOB_RECURSE QOBJECT_SOURCES
    "*.cpp"
)

file(GLOB_RECURSE QOBJECT_IMAGES
    "images/*.png"
)

QT5_WRAP_CPP(hdr_moc ${QOBJECT_HEADERS})
#qt5_add_resources(mainresource application.qrc)

  # Executables fail to build with Qt 5 in the default configuration
  # without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}  -std=c++11 -Wall")

ADD_LIBRARY(libQtGUI
    ${QOBJECT_SOURCES}
	${hdr_moc} # The MOC headers, generated
	${mainresource}	# The resource file of the main window
)

# Use the Widgets module from Qt 5.
target_link_libraries(libQtGUI #libEMPA
Qt5::Widgets Qt5::PrintSupport)

set_target_properties(libQtGUI
                      PROPERTIES OUTPUT_NAME QtGUI
                      )

target_include_directories(libQtGUI PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  ${QOBJECT_SOURCES} application.qrc CMakeLists.txt RegisterWidget.ui
	DESTINATION QtGUI
	COMPONENT srcs)

INSTALL(FILES  ${QOBJECT_HEADERS} 
	DESTINATION QtGUI/include
	COMPONENT srcs)

INSTALL(FILES  ${QOBJECT_IMAGES}  images/empa-logo-small.ico
	DESTINATION QtGUI/images
	COMPONENT srcs)
