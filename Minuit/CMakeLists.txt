# This is the main CMakeLists file of the Minuit library
#
# @author János Végh

# Generate the Minuit minimization library

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} Minuit library")
file(GLOB_RECURSE MY_SRCS
    "*.h"
    "*.cpp"
)
                      
include_directories( include
		)

ADD_LIBRARY(libMinuit
    ${MY_SRCS}
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
