# This is the main CMakeLists file of the Minuit library
#
# @author János Végh

# Generate the Minuit minimization library

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} Minuit library")
file(GLOB_RECURSE MOBJECT_HEADERS
    "include/*.h"
)

file(GLOB_RECURSE MOBJECT_SOURCES
    "*.cpp"
)

include_directories( include
		)

ADD_LIBRARY(libMinuit
    ${MOBJECT_SOURCES}
)

target_link_libraries(
    libMinuit
    ${Caffe_LINKER_LIBS}
    ${Pthread}
 )

set_target_properties(
    libMinuit
    PROPERTIES OUTPUT_NAME Minuit
    )

target_include_directories(
    libMinuit PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  ${MOBJECT_SOURCES} CMakeLists.txt
	DESTINATION Minuit
	COMPONENT srcs)

INSTALL(FILES  ${MOBJECT_HEADERS} 
	DESTINATION Minuit/include
	COMPONENT srcs)


