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
#include "mocked_mac_callbacks.h"
#include "sf_serialmac.h"
#include "sf_crc.h"


/*======================================================================================
 * Custom Matchers
 *======================================================================================*/
bool BufferMatcher(uint8_t *buff, uint8_t *matchBuff, size_t matchBuffLen) {

    if(!buff) {
        return false;
    }

    int i;
    for(i=0; i<matchBuffLen; i++) {
        if(buff[i] != matchBuff[i]) {
            break;
        }
    }

    return i == matchBuffLen;
}

const uint SerialMacTest::macSyncWordFieldLength = SF_SERIALMAC_PROTOCOL_SYNC_WORD_LEN;
const uint SerialMacTest::macCrcFieldLength = SF_SERIALMAC_PROTOCOL_CRC_FIELD_LEN;

SerialMacTest::SerialMacTest() {
    serialMacCtxt = (sf_serialmac_ctx*)std::malloc(sf_serialmac_ctx_size());
    invertedLengthField = true;
    setMockMac_callbacksPointer(&macCallbacksMock);
    crcBuffer = (uint8_t*)std::malloc(macCrcFieldLength);
}

SerialMacTest::~SerialMacTest() {
    std::free(serialMacCtxt);
    std::free(crcBuffer);
    setMockMac_callbacksPointer(nullptr);
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
