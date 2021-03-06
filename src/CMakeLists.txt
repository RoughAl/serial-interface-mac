##
## @code
##  ___ _____ _   ___ _  _____ ___  ___  ___ ___
## / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
## \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
## |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
## embedded.connectivity.solutions.==============
## @endcode
##
## @file
## @copyright  STACKFORCE GmbH, Heitersheim, Germany, www.stackforce.de
## @author     Lars Moellendorf
## @author     Adrian Antonana
## @author     Johannes Bruder
## @brief      STACKFORCE Serial MAC Module
##
## This file is part of the STACKFORCE Serial MAC Library
## (below "libsfserialmac").
##
## libsfserialmac is free software: you can redistribute it and/or modify
## it under the terms of the GNU Affero General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## libsfserialmac is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Affero General Public License for more details.
##
## You should have received a copy of the GNU Affero General Public License
## along with libsfserialmac.  If not, see <http://www.gnu.org/licenses/>.
##

#---------------------------------------------------------------------------------------
# Serialmac headers and sources
#---------------------------------------------------------------------------------------
set(SERIALMAC_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/sf_serialmac.c
)

# configure version header
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/version.h.in
    ${PROJECT_BINARY_DIR}/version.h
)

#---------------------------------------------------------------------------------------
# Serialmac targets
#---------------------------------------------------------------------------------------

# runtime configurable
add_library(${PROJECT_NAME}_o OBJECT ${SERIALMAC_SOURCES})
add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_o>)
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_o>)

target_include_directories(${PROJECT_NAME}_o PRIVATE $<TARGET_PROPERTY:sfcrc,INTERFACE_INCLUDE_DIRECTORIES>)

set_target_properties(${PROJECT_NAME}_o PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME}-static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/sfserialmac>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)

target_include_directories(${PROJECT_NAME}-static INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/sfserialmac>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME}-static INTERFACE sfcrc-static)
target_link_libraries(${PROJECT_NAME} PUBLIC sfcrc)

#---------------------------------------------------------------------------------------
# Doxygen documentation generation
#---------------------------------------------------------------------------------------

if(BUILD_DOC)

    # variables for use by generic doxygen configuration file
    set(DOXYGEN_PRODUCT_NAME "${PROJECT_PRODUCT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/doc)
    set(DOXYGEN_CUSTOM_RESOURCES_DIR ${PROJECT_BINARY_DIR}/doxygen)
    set(DOWNLOADED_MODUELES_DIR ${PROJECT_BINARY_DIR}/cmake)

    string(APPEND DOXYGEN_INPUT ${PROJECT_SOURCE_DIR}/README.md " ")
    string(APPEND DOXYGEN_INPUT ${CMAKE_CURRENT_LIST_DIR} " ")
    string(APPEND DOXYGEN_INPUT ${PROJECT_BINARY_DIR})

    message(STATUS "doxyfile input: ${DOXYGEN_INPUT}")

    set(SF_CORPORATE_DESIGN_URL "https://raw.githubusercontent.com/stackforce/corporate-design-doxygen/master")
    message(STATUS "Download files for STACKFORCE doxygen corporate design:")

    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/cmake/DoxygenRequiredVersion.cmake"
        "${DOWNLOADED_MODUELES_DIR}/DoxygenRequiredVersion.cmake"
        SHOW_PROGRESS
    )

    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/Doxyfile.in"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/Doxyfile.in"
        SHOW_PROGRESS
    )

    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/bootstrap.min.css"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/bootstrap.min.css"
        SHOW_PROGRESS
    )
    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/sf_stylesheet.css"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/sf_stylesheet.css"
        SHOW_PROGRESS
    )
    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/sf_footer.html"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/sf_footer.html"
        SHOW_PROGRESS
    )
    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/sf_header.html"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/sf_header.html"
        SHOW_PROGRESS
    )
    file(DOWNLOAD
        "${SF_CORPORATE_DESIGN_URL}/stackforce_logo.svg"
        "${DOXYGEN_CUSTOM_RESOURCES_DIR}/stackforce_logo.svg"
        SHOW_PROGRESS
    )

    list(APPEND CMAKE_MODULE_PATH ${DOWNLOADED_MODUELES_DIR})
    include(DoxygenRequiredVersion)

    configure_file(${DOXYGEN_CUSTOM_RESOURCES_DIR}/Doxyfile.in
        ${PROJECT_BINARY_DIR}/Doxyfile @ONLY
    )

    # set the readme file as mainpage
    file(APPEND ${PROJECT_BINARY_DIR}/Doxyfile
        "USE_MDFILE_AS_MAINPAGE = README.md\n"
    )

    # Ensure that the documentation is build for C code
    file(APPEND ${PROJECT_BINARY_DIR}/Doxyfile
        "OPTIMIZE_OUTPUT_FOR_C  = YES\n"
    )

    add_custom_target(doc ALL
        ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        SOURCES ${PROJECT_BINARY_DIR}/Doxyfile
    )

    install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/html DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME})
endif()

#---------------------------------------------------------------------------------------
# Install/export targets and files
#---------------------------------------------------------------------------------------
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(PROJECT_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(PROJECT_CONFIG_FILE "${PROJECT_NAME}-config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# generate package version file
write_basic_package_version_file(
    ${PROJECT_VERSION_FILE}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# install targets
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static
        EXPORT ${TARGETS_EXPORT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install headers
install(FILES ${CMAKE_CURRENT_LIST_DIR}/sf_serialmac.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# install version file
install(FILES ${PROJECT_VERSION_FILE}
        DESTINATION ${CONFIG_INSTALL_DIR}
)

# build directory package config
export(EXPORT ${TARGETS_EXPORT_NAME}
       FILE ${PROJECT_CONFIG_FILE}
)

# installed package config
install(EXPORT ${TARGETS_EXPORT_NAME}
        DESTINATION ${CONFIG_INSTALL_DIR}
        FILE ${PROJECT_CONFIG_FILE}
)

# add to CMake registry
export(PACKAGE ${PROJECT_NAME})

#---------------------------------------------------------------------------------------
# Create release packages
#---------------------------------------------------------------------------------------
# package release version
set(PACKAGE_RELEASE_VERSION 1)

# determine distribution and architecture
execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE LINUX_DISTRO OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)

# default CPack generators
set(CPACK_GENERATOR TGZ STGZ)

# extra CPack generators
if(LINUX_DISTRO STREQUAL "Debian" OR LINUX_DISTRO STREQUAL "Ubuntu" OR LINUX_DISTRO STREQUAL "LinuxMint")
    list(APPEND CPACK_GENERATOR DEB)
endif()

# general CPack config
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)
message(STATUS "CPack: packages will be generated under ${CPACK_PACKAGE_DIRECTORY}")
set(CPACK_PACKAGE_NAME "lib${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_PRODUCT_COMPANY} ${PROJECT_PRODUCT_NAME}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_PRODUCT_COMPANY} GmbH")
set(CPACK_PACKAGE_CONTACT "info@stackforce.de")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_RELEASE_VERSION}-Linux-${MACHINE_ARCH})
set(CPACK_STRIP_FILES ON)

# debian specific CPack config
set(CPACK_DEBIAN_PACKAGE_RELEASE ${PACKAGE_RELEASE_VERSION})
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfcrc (= ${SFCRC_VERSION})")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/stackforce")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

include(CPack)
