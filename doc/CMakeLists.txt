# This is the CMakeLists file for documenting the ESsystMe components
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh


# Generate the Doxygen documentation
#https://tty1.net/blog/2014/cmake-doxygen_en.html

# ---[ Configuring  Doxygen
 find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)"
${DOXYGEN_FOUND})
if (DOXYGEN_FOUND)
if(NEED_DOCS)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the .html documentation.")
    endif()
    FIND_PROGRAM(DPKG_CHMCMD_FOUND chmcmd)
    if(NOT DPKG_CHMCMD_FOUND)
        message(FATAL_ERROR "chmcmd is needed to build the .chm documentation.")
    endif(NOT DPKG_CHMCMD_FOUND)
    message(HIGHLIGHTED "Configuring ${PROJECT_NAME} documentation")

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(doc
        DEPENDS ${PROJECT_NAME} #${PROJECT_NAME}_TEST
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile} 
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
        
    if(DPKG_CHMCMD_FOUND)
      message(HIGHLIGHTED "Program chmcmd found")
      add_custom_target(chm
        DEPENDS doc 
 	COMMAND chmcmd index.hhp
 	COMMAND chmod 755 ${PROJECT_NAME}.chm
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
        COMMENT "Generating API documentation with chmcmd using Doxygen output"
        VERBATIM)

      add_dependencies(chm doc)     
     endif(DPKG_CHMCMD_FOUND)
endif(NEED_DOCS)
else()
  message(AUTHOR_WARNING "Doxygen not found")
endif (DOXYGEN_FOUND)

INSTALL(FILES ${CMAKE_BINARY_DIR}/doc/${PROJECT_NAME}.chm 
	DESTINATION doc
 	COMPONENT docs)
file(GLOB_RECURSE DOCSFILES
    "*.dox"
)

file(GLOB_RECURSE IMAGEFILES
    "images/*.png"
)
  INSTALL(FILES  ${DOCSFILES} CMakeLists.txt documentation-config.doxygen.in ESsistMe.bib
	DESTINATION doc
	COMPONENT srcs)

  INSTALL(FILES  ${IMAGEFILES}
	DESTINATION doc/images
	COMPONENT srcs)
