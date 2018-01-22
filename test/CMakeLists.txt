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

#---------------------------------------------------------------------------------------
# Common sources
#---------------------------------------------------------------------------------------
set(TEST_SUITE_COMMON_SOURECES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_test.cpp
)

#=======================================================================================
# Initialization Test Suites
#=======================================================================================

#---------------------------------------------------------------------------------------
# Configurable library
#---------------------------------------------------------------------------------------
set(TEST_SUITE_CONFIGURABLE_INIT_NAME serialmac-configurable-init-test-suite)

set(TEST_SUITE_CONFIGURABLE_INIT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_init_suite.cpp
)

add_executable(${TEST_SUITE_CONFIGURABLE_INIT_NAME}
    ${TEST_SUITE_COMMON_SOURECES}
    ${TEST_SUITE_CONFIGURABLE_INIT_SOURCES}
)

target_link_libraries(${TEST_SUITE_CONFIGURABLE_INIT_NAME}
    libgmock
    sfserialmac-configurable
)

add_test(
    NAME ${TEST_SUITE_CONFIGURABLE_INIT_NAME}
    COMMAND ${TEST_SUITE_CONFIGURABLE_INIT_NAME} --gtest_output=xml:${TEST_SUITE_CONFIGURABLE_INIT_NAME}-xml
)

#---------------------------------------------------------------------------------------
# Non-configurable library
#---------------------------------------------------------------------------------------
set(TEST_SUITE_NO_CONFIGURABLE_INIT_NAME serialmac-no-configurable-init-test)

set(TEST_SUITE_NO_CONFIGURABLE_INIT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_no_configurable_init_suite.cpp
)

add_executable(${TEST_SUITE_NO_CONFIGURABLE_INIT_NAME}
    ${TEST_SUITE_COMMON_SOURECES}
    ${TEST_SUITE_NO_CONFIGURABLE_INIT_SOURCES}
)

target_link_libraries(${TEST_SUITE_NO_CONFIGURABLE_INIT_NAME}
    libgmock
    sfserialmac
)

add_test(
    NAME ${TEST_SUITE_NO_CONFIGURABLE_INIT_NAME}
    COMMAND ${TEST_SUITE_NO_CONFIGURABLE_INIT_NAME} --gtest_output=xml:${TEST_SUITE_NO_CONFIGURABLE_INIT_NAME}-xml
)

#=======================================================================================
# Transmission Test Suites
#=======================================================================================

#---------------------------------------------------------------------------------------
# Configurable library with enabled inverted length field
#---------------------------------------------------------------------------------------
set(TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME serialmac-configurable-inverted-length-tx-test-suite)

set(TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_inverted_length_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_inverted_length_tx_suite.cpp
)

add_executable(${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME}
    ${TEST_SUITE_COMMON_SOURECES}
    ${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_SOURCES}
)

target_link_libraries(${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME}
    libgmock
    sfserialmac-configurable
)

add_test(
    NAME ${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME}
    COMMAND ${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME} --gtest_output=xml:${TEST_SUITE_CONFIGURABLE_INV_LENGTH_TX_NAME}-xml
)

#---------------------------------------------------------------------------------------
# Configurable library with disabled inverted length field
#---------------------------------------------------------------------------------------
set(TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME serialmac-configurable-no-inverted-length-tx-test-suite)

set(TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_no_inverted_length_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sf_serialmac_no_inverted_length_tx_suite.cpp
)

add_executable(${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME}
    ${TEST_SUITE_COMMON_SOURECES}
    ${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_SOURCES}
)

target_link_libraries(${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME}
    libgmock
    sfserialmac-configurable
)

add_test(
    NAME ${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME}
    COMMAND ${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME} --gtest_output=xml:${TEST_SUITE_CONFIGURABLE_NO_INV_LENGTH_TX_NAME}-xml
)
