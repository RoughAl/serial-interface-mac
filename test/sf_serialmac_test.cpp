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
