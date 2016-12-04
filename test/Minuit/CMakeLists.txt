# This is the CMakeLists file for testing the Minuit component
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} Minuit tests")

# ---[ Headers for Minuit tests
include_directories(
    ${CMAKE_SOURCE_DIR}/Minuit/include
    ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim
    ${CMAKE_SOURCE_DIR}/main/include
    )

# ---[ Build executables for Minuit tests
# The tests are unchanged, except that
# files moved to "tests"
# subdirectory names from "include"s removed
# some missing "include"s (mainly <cstdlib>) added
ADD_EXECUTABLE(Quad1FMain_TEST MnTutorial/Quad1FMain.cpp)
target_link_libraries(Quad1FMain_TEST libMinuit)
install(TARGETS Quad1FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad1FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad1.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

ADD_EXECUTABLE(Quad12FMain_TEST MnTutorial/Quad12FMain.cpp)
target_link_libraries(Quad12FMain_TEST libMinuit)
install(TARGETS Quad12FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad12FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad12.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

ADD_EXECUTABLE(Quad2FMain_TEST MnTutorial/Quad2FMain.cpp)
target_link_libraries(Quad2FMain_TEST libMinuit)
install(TARGETS Quad2FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad2FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad2.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

ADD_EXECUTABLE(Quad3FMain_TEST MnTutorial/Quad3FMain.cpp)
target_link_libraries(Quad3FMain_TEST libMinuit)
install(TARGETS Quad3FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad3FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad3.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

ADD_EXECUTABLE(Quad4FMain_TEST MnTutorial/Quad4FMain.cpp)
target_link_libraries(Quad4FMain_TEST libMinuit)
install(TARGETS Quad4FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad4FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad4.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

ADD_EXECUTABLE(Quad8FMain_TEST MnTutorial/Quad8FMain.cpp)
target_link_libraries(Quad8FMain_TEST libMinuit)
install(TARGETS Quad8FMain_TEST  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad4FMain.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnTutorial/Quad4.h
	DESTINATION test/Minuit/MnTutorial COMPONENT tests)

# ---[ Build executables for Minuit demos/tests
# The demos are unchanged, except that
#   files moved to "tests"
#   subdirectory names from "include"s removed
#   some missing "include"s (mainly <cstdlib>) added
#   linking added
ADD_EXECUTABLE(DemoGaussSim MnSim/DemoGaussSim.cpp MnSim/GaussFcn MnSim/GaussDataGen)
target_link_libraries(DemoGaussSim libMinuit)
install(TARGETS DemoGaussSim  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/DemoGaussSim.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/GaussDataGen.h
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/GaussFcn.h
	DESTINATION test/Minuit/MnSim COMPONENT tests)

ADD_EXECUTABLE(PaulTest MnSim/PaulTest.cpp MnSim/GaussFcn)
target_link_libraries(PaulTest libMinuit)
install(TARGETS PaulTest  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/PaulTest.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/GaussFcn.h
	DESTINATION test/Minuit/MnSim COMPONENT tests)

ADD_EXECUTABLE(PaulTest2 MnSim/PaulTest2.cpp MnSim/GaussFcn)
target_link_libraries(PaulTest2 libMinuit)
install(TARGETS PaulTest2  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/PaulTest2.cpp
              ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/GaussFcn2.h
	DESTINATION test/Minuit/MnSim COMPONENT tests)

ADD_EXECUTABLE(PaulTest3 MnSim/PaulTest3.cpp MnSim/GaussFcn2)
target_link_libraries(PaulTest3 libMinuit)
install(TARGETS PaulTest3  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/PaulTest3.cpp
	DESTINATION test/Minuit/MnSim COMPONENT tests)

ADD_EXECUTABLE(PaulTest4 MnSim/PaulTest4.cpp)
target_link_libraries(PaulTest4 libMinuit)
install(TARGETS PaulTest4  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/PaulTest4.cpp
	DESTINATION test/Minuit/MnSim COMPONENT tests)

ADD_EXECUTABLE(ReneTest MnSim/ReneTest.cpp)
target_link_libraries(ReneTest libMinuit)
install(TARGETS ReneTest  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnSim/ReneTest.cpp
	DESTINATION test/Minuit/MnSim COMPONENT tests)


# ---[ Build gtest executables for Minuit
set(TEST_SOURCES
    testMinuit.cpp	# The main program
    MnLink/LinkingTest.cpp
    MnSim/GaussDataGen.cpp
    MnSim/GaussFcn.cpp
    ${CMAKE_SOURCE_DIR}/main/Stuff.cpp
)

ADD_EXECUTABLE(testMinuit ${TEST_SOURCES})
target_link_libraries(testMinuit libMinuit
    ${GLOG_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${Caffe_LINKER_LIBS}
    ${Pthread}
)

install(TARGETS testMinuit  RUNTIME  DESTINATION bin COMPONENT tests)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/test/Minuit/MnLink/LinkingTest.cpp
	DESTINATION test/Minuit/MnSim COMPONENT tests)

INSTALL(FILES ${TEST_SOURCES} CMakeLists.txt
	DESTINATION test
	COMPONENT srcs)

	