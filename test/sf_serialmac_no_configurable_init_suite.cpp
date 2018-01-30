/**
 * @code
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions.==============
 * @endcode
 *
 * @file
 * @copyright  STACKFORCE GmbH, Heitersheim, Germany, http://www.stackforce.de
 * @author     Adrian Antonana <adrian.antonana@stackforce.de>
 * @brief      Non configurable Serial MAC init test suite.
 */

#include "sf_serialmac_inverted_length_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

/**
 * @brief Test to initialize the allowed inverted field value.
 */
TEST_F(SerialMacInvertedLengthTest, CorrectInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                &dummyPortHandle,
                                (SF_SERIALMAC_HAL_READ_FUNCTION) HalReadCb,
                                (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) HalReadWaitingCb,
                                (SF_SERIALMAC_HAL_WRITE_FUNCTION) HalWriteCb,
                                (SF_SERIALMAC_EVENT) ReadFrameCb,
                                (SF_SERIALMAC_EVENT) ReadBufferCb,
                                (SF_SERIALMAC_EVENT) ReadSyncByteCb,
                                (SF_SERIALMAC_EVENT) WriteFrameCb,
                                (SF_SERIALMAC_EVENT) WriteBufferCb,
                                (SF_SERIALMAC_EVENT_ERROR) MacErrorCb,
                                true);

    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Failed to initialize serial mac";
}

/**
 * @brief Test attempt to initialize the inverted field to not allowed value.
 */
TEST_F(SerialMacInvertedLengthTest, WrongInvertedLengthInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                &dummyPortHandle,
                                (SF_SERIALMAC_HAL_READ_FUNCTION) HalReadCb,
                                (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) HalReadWaitingCb,
                                (SF_SERIALMAC_HAL_WRITE_FUNCTION) HalWriteCb,
                                (SF_SERIALMAC_EVENT) ReadFrameCb,
                                (SF_SERIALMAC_EVENT) ReadBufferCb,
                                (SF_SERIALMAC_EVENT) ReadSyncByteCb,
                                (SF_SERIALMAC_EVENT) WriteFrameCb,
                                (SF_SERIALMAC_EVENT) WriteBufferCb,
                                (SF_SERIALMAC_EVENT_ERROR) MacErrorCb,
                                false);

    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_UNSUPPORTED_PARAMETER)
    << "Mac init should have failed";
}
