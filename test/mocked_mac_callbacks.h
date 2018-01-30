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
 * @brief      Serial MAC mocked callback functions
 */

#ifndef _MOCK_MAC_CALLBACKS_H_
#define _MOCK_MAC_CALLBACKS_H_

#include <stdint.h>
#include <stddef.h>
#include <gmock/gmock.h>

#include "sf_serialmac.h"

/**
 * Serial MAC callback functions to be registered by the MAC's init function. These are free functions since
 * C-style callbacks can not be C++ class member methods.
 * Calls to these functions will be forwarded to the according mocked methods from the MockMacCallbacks class.
 */
void ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
void ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
void WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
void WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
void ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize);
size_t HalReadWaitingCb(void *portHandle);
size_t HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize);
size_t HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize);
void MacErrorCb(struct sf_serialmac_ctx *serialMacCtxt, sf_serialmac_error e);

/**
 * Mac callbacks interface class.
 */
class MacCallbacks {

    public:
        virtual ~MacCallbacks() {}
        virtual size_t HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize) = 0;
        virtual size_t HalReadWaitingCb(void *portHandle) = 0;
        virtual size_t HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize) = 0;
        virtual void MacErrorCb(struct sf_serialmac_ctx *serialMacCtxt, sf_serialmac_error e) = 0;
        virtual void ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) = 0;
        virtual void ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) = 0;
        virtual void WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) = 0;
        virtual void WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) = 0;
        virtual void ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) = 0;
};

/**
 * Mac callbacks mock class.
 */
class MockMacCallbacks : public MacCallbacks {

    public:
        MOCK_METHOD3(HalReadCb, size_t(void *portHandle, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD1(HalReadWaitingCb,  size_t(void *portHandle));
        MOCK_METHOD3(HalWriteCb, size_t(void *portHandle, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD2(MacErrorCb, void(struct sf_serialmac_ctx *serialMacCtxt, sf_serialmac_error e));
        MOCK_METHOD3(ReadBufferCb, void(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD3(ReadSyncByteCb, void(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD3(WriteBufferCb, void(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD3(WriteFrameCb, void(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize));
        MOCK_METHOD3(ReadFrameCb, void(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize));
};

void setMockMac_callbacksPointer(MockMacCallbacks *macCallbacksRef);

#endif
