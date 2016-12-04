# This is the CMakeLists file for testing the ESsistMe components
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} tests")

include_directories( include
    ${CMAKE_SOURCE_DIR}/main/include
    ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim
    ${CMAKE_SOURCE_DIR}/Minuit/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Minuit/MnSim
    )

set(TEST_SOURCES
    testESsistMe_Units.cpp	# The main program
    StuffTest.cpp
    Minuit/MnLink/LinkingTest.cpp
    Minuit/MnSim/GaussDataGen.cpp
    Minuit/MnSim/GaussFcn.cpp
    ${CMAKE_SOURCE_DIR}/main/Stuff.cpp
)
		
ADD_EXECUTABLE(
    ${PROJECT_NAME}_TEST
    ${TEST_SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}_TEST libMinuit
    ${GLOG_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${Caffe_LINKER_LIBS}
    ${Pthread}
)
  install(TARGETS ${PROJECT_NAME}_TEST 
   RUNTIME
   DESTINATION bin
   COMPONENT tests)

#  install(DIRECTORY DESTINATION ${CPACK_PACKAGE_NAME}/output
#   COMPONENT tests)

#  INSTALL(FILES ${CMAKE_SOURCE_DIR}/EMPAfiles/source/QTests.Eys ${CMAKE_SOURCE_DIR}/EMPAfiles/source/QTests.Eyo
#	DESTINATION EMPAfiles/test
#	COMPONENT tests)

  INSTALL(FILES ${TEST_SOURCES} CMakeLists.txt
	DESTINATION test
	COMPONENT srcs)

	