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

#include <gmock/gmock.h>
#include "sf_serialmac.h"

#include "mocked_mac_callbacks.h"

/* Mocked mac callbacks reference. */
MockMacCallbacks *macCallbacksMock;

size_t HalReadCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    return macCallbacksMock->HalReadCb( portHandle, buffer, bufferSize);
}

size_t HalReadWaitingCb(void *portHandle) {
    assert(macCallbacksMock != NULL);
    return macCallbacksMock->HalReadWaitingCb( portHandle);
}

size_t HalWriteCb(void *portHandle, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    return macCallbacksMock->HalWriteCb( portHandle, buffer, bufferSize);
}

void MacErrorCb(struct sf_serialmac_ctx *serialMacCtxt, sf_serialmac_error e) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->MacErrorCb( serialMacCtxt, e);
}

void ReadBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->ReadBufferCb( serialMacCtxt, buffer, bufferSize);
}

void ReadSyncByteCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->ReadSyncByteCb( serialMacCtxt, buffer, bufferSize);
}

void WriteBufferCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->WriteBufferCb( serialMacCtxt, buffer, bufferSize);
}

void WriteFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->WriteFrameCb( serialMacCtxt, buffer, bufferSize);
}

void ReadFrameCb(struct sf_serialmac_ctx *serialMacCtxt, uint8_t *buffer, size_t bufferSize) {
    assert(macCallbacksMock != NULL);
    macCallbacksMock->ReadFrameCb( serialMacCtxt, buffer, bufferSize);
}

void setMockMac_callbacksPointer(MockMacCallbacks *macCallbacksRef) {
    macCallbacksMock = macCallbacksRef;
}

