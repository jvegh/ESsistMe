# This is the CMakeLists file for testing the ESsistMe components
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} tests")

include_directories( include
    ${CMAKE_SOURCE_DIR}/main/include
    ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim
    ${CMAKE_SOURCE_DIR}/Minuit/include
    ${CMAKE_SOURCE_DIR}/SPeval/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Minuit/MnSim
    )

set(TEST_SOURCES
    testESsistMe_Units.cpp	# The main program
    StuffTest.cpp
    ComponentTest.cpp
    EvalTest.cpp
    SpectrumTest.cpp
)
		
ADD_EXECUTABLE(
    ${PROJECT_NAME}_TEST
    ${TEST_SOURCES}
    Minuit/MnLink/LinkingTest.cpp
    Minuit/MnSim/GaussDataGen.cpp
    Minuit/MnSim/GaussFcn.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_TEST libMain libMinuit libSPeval
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


  INSTALL(FILES ${TEST_SOURCES} CMakeLists.txt
	DESTINATION test
	COMPONENT srcs)

  INSTALL(FILES Minuit/MnLink/LinkingTest.cpp
	DESTINATION Minuit/MnSim
	COMPONENT srcs)

  INSTALL(FILES Minuit/MnSim/GaussDataGen.cpp Minuit/MnSim/GaussFcn.cpp
	DESTINATION test/Minuit/MnSim
	COMPONENT srcs)

  INSTALL(FILES Minuit/CMakeLists.txt Minuit/testMinuit.cpp
	DESTINATION test/Minuit
	COMPONENT srcs)

  INSTALL(FILES Minuit/MnLink/DemoLink.cpp Minuit/MnLink/LinkingTest.cpp 
	Minuit/MnLink/LinkingTest.h
	DESTINATION test/Minuit/MnLink/
	COMPONENT srcs)

file(GLOB_RECURSE MnSimFiles
    "Minuit/MnSim/*.h" "Minuit/MnSim/*.cpp" "Minuit/MnSim/*.txt"
)

  INSTALL(FILES ${MnSimFiles}
	DESTINATION test/Minuit/MnSim/
	COMPONENT srcs)

file(GLOB_RECURSE MnTutFiles
    "Minuit/MnTutorial/*.h" "Minuit/MnTutorial/*.cpp" 
)

  INSTALL(FILES ${MnTutFiles}
	DESTINATION test/Minuit/MnTutorial/
	COMPONENT srcs)


