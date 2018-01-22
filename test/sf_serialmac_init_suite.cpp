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
 * @brief      Serial MAC init test suite.
 */

#include "sf_serialmac_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

/**
 * @brief Test correct serial mac initialization.
 */
TEST_F(SerialMacTest, CorrectInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                dummyPortHandle,
                                (SF_SERIALMAC_HAL_READ_FUNCTION) SerialMacTest::HalReadCb,
                                (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) SerialMacTest::HalReadWaitingCb,
                                (SF_SERIALMAC_HAL_WRITE_FUNCTION) SerialMacTest::HalWriteCb,
                                (SF_SERIALMAC_EVENT) SerialMacTest::ReadFrameCb,
                                (SF_SERIALMAC_EVENT) SerialMacTest::ReadBufferCb,
                                (SF_SERIALMAC_EVENT) SerialMacTest::ReadSyncByteCb,
                                (SF_SERIALMAC_EVENT) SerialMacTest::WriteFrameCb,
                                (SF_SERIALMAC_EVENT) SerialMacTest::WriteBufferCb,
                                (SF_SERIALMAC_EVENT_ERROR) SerialMacTest::MacErrorCb,
                                invertedLengthField);

    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Failed to initialize serial mac";
}

/**
 * @brief Test attempt to init with a NULL port handle.
 */
TEST_F(SerialMacTest, WrongPortHandleInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                    NULL,
                                    (SF_SERIALMAC_HAL_READ_FUNCTION) SerialMacTest::HalReadCb,
                                    (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) SerialMacTest::HalReadWaitingCb,
                                    (SF_SERIALMAC_HAL_WRITE_FUNCTION) SerialMacTest::HalWriteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadBufferCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadSyncByteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteBufferCb,
                                    (SF_SERIALMAC_EVENT_ERROR) SerialMacTest::MacErrorCb,
                                    invertedLengthField);

    EXPECT_NE(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Mac init should have failed";
}

/**
 * @brief Test attempt to init with a NULL HAL-read callback function.
 */
TEST_F(SerialMacTest, WrongHalReadInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                    dummyPortHandle,
                                    NULL,
                                    (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) SerialMacTest::HalReadWaitingCb,
                                    (SF_SERIALMAC_HAL_WRITE_FUNCTION) SerialMacTest::HalWriteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadBufferCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadSyncByteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteBufferCb,
                                    (SF_SERIALMAC_EVENT_ERROR) SerialMacTest::MacErrorCb,
                                    invertedLengthField);

    EXPECT_NE(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Mac init should have failed";
}

/**
 * @brief Test attempt to init with a NULL HAL-bytes-waiting-for-read callback function.
 */
TEST_F(SerialMacTest, WrongHalReadWaitingInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                    dummyPortHandle,
                                    (SF_SERIALMAC_HAL_READ_FUNCTION) SerialMacTest::HalReadCb,
                                    NULL,
                                    (SF_SERIALMAC_HAL_WRITE_FUNCTION) SerialMacTest::HalWriteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadBufferCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadSyncByteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteBufferCb,
                                    (SF_SERIALMAC_EVENT_ERROR) SerialMacTest::MacErrorCb,
                                    invertedLengthField);

    EXPECT_NE(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Mac init should have failed";
}

/**
 * @brief Test attempt to init with a NULL HAL-write callback function.
 */
TEST_F(SerialMacTest, WrongHalWriteInit) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_init (serialMacCtxt,
                                    dummyPortHandle,
                                    (SF_SERIALMAC_HAL_READ_FUNCTION) SerialMacTest::HalReadCb,
                                    (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) SerialMacTest::HalReadWaitingCb,
                                    NULL,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadBufferCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::ReadSyncByteCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteFrameCb,
                                    (SF_SERIALMAC_EVENT) SerialMacTest::WriteBufferCb,
                                    (SF_SERIALMAC_EVENT_ERROR) SerialMacTest::MacErrorCb,
                                    invertedLengthField);

    EXPECT_NE(macRet, SF_SERIALMAC_RETURN_SUCCESS)
    << "Mac init should have failed";
}
