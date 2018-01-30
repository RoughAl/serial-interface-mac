/**
 * @code
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions.==============
 * @endcode
 *
 * @copyright  STACKFORCE GmbH, Heitersheim, Germany, http://www.stackforce.de
 * @author     Adrian Antonana <adrian.antonana@stackforce.de>
 * @brief      Serial MAC unit tests base class
 */

#include <iomanip>

#include "sf_serialmac_test.h"
#include "sf_serialmac.h"
#include "sf_crc.h"

std::vector<uint8_t> SerialMacTest::fullSentTestBuffer;
size_t SerialMacTest::halRxBytesWaiting;
std::vector<uint8_t> SerialMacTest::halBuffer;
std::vector<uint8_t>::iterator SerialMacTest::itHalBuffer;
uint8_t *SerialMacTest::payloadBuffer;
std::vector<uint8_t> SerialMacTest::rxPayload;
enum sf_serialmac_error SerialMacTest::macError;

void SerialMacTest::ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

    SerialMacTest::rxPayload.clear();
    SerialMacTest::rxPayload.assign(buffer, buffer + bufferSize);

    std::free(SerialMacTest::payloadBuffer);
};

void SerialMacTest::ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

    SerialMacTest::payloadBuffer = (uint8_t*)std::malloc(bufferSize);

    if(sf_serialmac_rx_frame(serialMacCtxt, SerialMacTest::payloadBuffer, bufferSize) != SF_SERIALMAC_RETURN_SUCCESS) {
        std::cout << "sf_serialmac_rx_frame returned error" << std::endl;
    }
};

void SerialMacTest::WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

void SerialMacTest::ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {

};

size_t SerialMacTest::HalReadWaitingCb(void *portHandle) {

    size_t halBufferRemainingBytes = 0;
    std::vector<uint8_t>::iterator it;

    for(it=SerialMacTest::itHalBuffer; it!=SerialMacTest::halBuffer.end(); it++) {
        halBufferRemainingBytes++;
    }

    return halBufferRemainingBytes;
};

size_t SerialMacTest::HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {

    int i;

    for(i=0; i<bufferSize; i++) {
        buffer[i] = *SerialMacTest::itHalBuffer;
        SerialMacTest::itHalBuffer++;
    }

    return bufferSize;
};

size_t SerialMacTest::HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {
    SerialMacTest::fullSentTestBuffer.insert(SerialMacTest::fullSentTestBuffer.end(), buffer, buffer+bufferSize);

    return bufferSize;
};

void SerialMacTest::MacErrorCb(void*, sf_serialmac_error e) {

    macError = e;
};

SerialMacTest::SerialMacTest() {
    serialMacCtxt = (sf_serialmac_ctx*)std::malloc(sf_serialmac_ctx_size());
    invertedLengthField = true;
    crcBuffer = (uint8_t*)std::malloc(SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN);
}

SerialMacTest::~SerialMacTest() {
    std::free(serialMacCtxt);
    std::free(crcBuffer);
}

void SerialMacTest::SetUp() {}
void SerialMacTest::TearDown() {}

void SerialMacTest::InitSerialMac() {
    sf_serialmac_init(serialMacCtxt,
                        (int*)&dummyPortHandle,
                        (SF_SERIALMAC_HAL_READ_FUNCTION) HalReadCb,
                        (SF_SERIALMAC_HAL_READ_WAIT_FUNCTION) HalReadWaitingCb,
                        (SF_SERIALMAC_HAL_WRITE_FUNCTION) HalWriteCb,
                        (SF_SERIALMAC_EVENT) ReadFrameCb,
                        (SF_SERIALMAC_EVENT) ReadBufferCb,
                        (SF_SERIALMAC_EVENT) ReadSyncByteCb,
                        (SF_SERIALMAC_EVENT) WriteFrameCb,
                        (SF_SERIALMAC_EVENT) WriteBufferCb,
                        (SF_SERIALMAC_EVENT_ERROR) MacErrorCb,
                        invertedLengthField);
}

void SerialMacTest::SetupFrameCrc(uint8_t *payload, uint16_t payloadLength) {
    uint16_t crc;
    crc = crc_calc_finalize(payload, payloadLength);

    crcBuffer[0] = (uint8_t)(crc >> 8);
    crcBuffer[1] = (uint8_t)crc;
}
