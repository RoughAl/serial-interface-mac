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
 * @brief      Serial MAC unit tests class
 */

#include <iomanip>

#include "sf_serialmac_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

std::vector<uint8_t> SerialMacTest::fullSentTestBuffer;
bool SerialMacTest::testingMacInit = true;

void SerialMacTest::ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::HalReadWaitingCb(void*) {

};

void SerialMacTest::HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {

};

size_t SerialMacTest::HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {
    SerialMacTest::fullSentTestBuffer.insert(SerialMacTest::fullSentTestBuffer.end(), buffer, buffer+bufferSize);

    return bufferSize;
};

void SerialMacTest::MacErrorCb(void*, sf_serialmac_error e) {

};

SerialMacTest::SerialMacTest() {
    serialMacCtxt = (sf_serialmac_ctx*)std::malloc(sf_serialmac_ctx_size());
    invertedLengthField = true;

    /* Initialize mac here if we're not testing it anymore */
    if(!testingMacInit) {
        InitSerialMac();
    }
}

SerialMacTest::~SerialMacTest() {
    std::free(serialMacCtxt);
}

void SerialMacTest::SetUp() {}
void SerialMacTest::TearDown() {}

void SerialMacTest::InitSerialMac() {
    sf_serialmac_init (serialMacCtxt,
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
}

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

TEST_F(SerialMacTest, SendFrameWrongMacCtxt) {

    /* No more mac init testing from here on, mac will be initialized
     * by the class constructor.
     */
    SerialMacTest::testingMacInit = false;

    sf_serialmac_return macRet;
    uint8_t txBuffer[3] = {0xAA, 0xBB, 0xCC};

    macRet = sf_serialmac_tx_frame(NULL, 3, txBuffer, 3);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

TEST_F(SerialMacTest, SendFrameWrongBuffer) {

    sf_serialmac_return macRet;

    macRet = sf_serialmac_tx_frame(serialMacCtxt, 3, NULL, 3);
    EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_ERROR_NPE)
    << "Frame tx should have failed";
}

TEST_F(SerialMacTest, SendFrame) {

    sf_serialmac_return macRet;
    uint8_t txBuffer[MAX_TEST_PAYLOAD_LEN];
    std::vector<uint8_t> expectedTxFullBufferVector;

    uint16_t i;
    uint16_t expectedCrc;

    for(i=0; i<MAX_TEST_PAYLOAD_LEN; i++) {
        txBuffer[i] = (uint8_t)i;
        expectedCrc = crc_calc_finalize(txBuffer, i+1);

        SerialMacTest::fullSentTestBuffer.clear();
        expectedTxFullBufferVector.clear();
        expectedTxFullBufferVector.push_back(SF_SERIALMAC_PROTOCOL_SYNC_WORD);
        expectedTxFullBufferVector.push_back((i+1) >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(i+1));
        expectedTxFullBufferVector.push_back(0xff - ((i+1) >> 8));
        expectedTxFullBufferVector.push_back(0xff - ((uint8_t)(i+1)));
        expectedTxFullBufferVector.insert(expectedTxFullBufferVector.end(), txBuffer, txBuffer+i+1);
        expectedTxFullBufferVector.push_back(expectedCrc >> 8);
        expectedTxFullBufferVector.push_back((uint8_t)(expectedCrc));

        macRet = sf_serialmac_tx_frame(serialMacCtxt, i+1, txBuffer, i+1);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx failed";

        macRet = sf_serialmac_hal_tx_callback(serialMacCtxt);
        EXPECT_EQ(macRet, SF_SERIALMAC_RETURN_SUCCESS)
        << "Frame tx callback failed";

        EXPECT_EQ(SerialMacTest::fullSentTestBuffer, expectedTxFullBufferVector)
        << "Unexpected frame sent";
    }
}
