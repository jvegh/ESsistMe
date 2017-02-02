# This is the main CMakeLists file of the SPeval library
#
# @author János Végh

# Generate the SPectrum evaluation library

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} SPeval library")
file(GLOB_RECURSE SOBJECT_HEADERS
    "include/*.h"
)

file(GLOB_RECURSE SOBJECT_SOURCES
    "*.cpp"
)

include_directories( include
    ${CMAKE_SOURCE_DIR}/Minuit/include
                )

ADD_LIBRARY(libSPeval
    ${SOBJECT_SOURCES}
)

target_link_libraries(
    libSPeval
    ${Caffe_LINKER_LIBS}
    ${Pthread}
 )

set_target_properties(
    libSPeval
    PROPERTIES OUTPUT_NAME SPeval
    )

target_include_directories(
    libSPeval PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  ${SOBJECT_SOURCES} CMakeLists.txt
	DESTINATION SPeval
	COMPONENT srcs)

INSTALL(FILES  ${SOBJECT_HEADERS} 
	DESTINATION SPeval/include
	COMPONENT srcs)


