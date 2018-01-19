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
## @author     Adrian Antonana
## @brief      STACKFORCE Serial MAC Module Unit Tests
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

set (CMAKE_CXX_STANDARD 11)

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_test.cpp
)

add_executable(serialmac-non-configurable-test ${TEST_SOURCES})

target_link_libraries(serialmac-non-configurable-test
    libgmock
    sfserialmac
)

add_test(
    NAME serialmac-non-configurable-test
    COMMAND serialmac-non-configurable-test --gtest_output=xml:serialmac-non-configurable-test-xml
)
