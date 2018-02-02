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

cmake_minimum_required(VERSION 3.6)
project(sfserialmac VERSION 3.0.1)

#---------------------------------------------------------------------------------------
# Include cmake modules
#---------------------------------------------------------------------------------------
list(APPEND CMAKE_FIND_ROOT_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# custom modules
include(ColorMessages)

#---------------------------------------------------------------------------------------
# Required dependencies versions
#---------------------------------------------------------------------------------------
set(SFCRC_VERSION 0.1.3)

#---------------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------------
option(BUILD_DOC "Build documentation" OFF)
option(BUILD_TESTING "Build unit tests" OFF)

#---------------------------------------------------------------------------------------
# Product info
#---------------------------------------------------------------------------------------
set(PROJECT_PRODUCT_COMPANY "STACKFORCE" CACHE STRING "company")
set(PROJECT_PRODUCT_NAME    "Serial MAC" CACHE STRING "product")

#---------------------------------------------------------------------------------------
# Find required packages
#---------------------------------------------------------------------------------------

# crc
colormessage(STATUS "Searching crc project configuration")
find_package(sfcrc ${CRC_VERSION} REQUIRED)

# googletest
if(BUILD_TESTING)
    colormessage(STATUS "Searching googletest project configuration")
    find_package(gtest REQUIRED CONFIG)
endif()


#---------------------------------------------------------------------------------------
# Add subdirectories
#---------------------------------------------------------------------------------------
add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

