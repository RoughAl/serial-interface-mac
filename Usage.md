# Serial Interface MAC Usage
```
     ___ _____ _   ___ _  _____ ___  ___  ___ ___
    / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
    \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
    |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
    embedded.connectivity.solutions.==============
```

# Usage

## Initialization

To use the STACKFORCE Serial MAC you have to initialize it using
**sf_serialmac_init()**

## Reacting to events

The STACKFORCE Serial MAC is event driven. You can use the MAC by calling
**sf_serialmac_entry()** periodically.

Or you can add **sf_serialmac_hal_tx_callback()** and
**sf_serialmac_hal_rx_callback()** as callback function to the corresponding
serial port events.

## Receiving frames

Whenever the STACKFORCE Serial MAC receives the header of a frame it calls
the upper layers callback function registered as **SF_SERIALMAC_RX_EVENT
rx_buffer_event()** on Initialization. To receive the frame the upper layer has
to provide a memory location for the payload passed to the MAC by calling
**sf_serialmac_rx_frame()**. As soon as the frame has been completed,
the upper layer's callback function is called which has been registered
as **SF_SERIALMAC_RX_EVENT rx_event()** on initialization.

In case there is any error or problem (e.g. invalid CRC) while receiving a frame,
the upper layer's error callback function is called. This function has been registered as
**SF_SERIALMAC_EVENT_ERROR error_event** on initialization.

## Transmitting frames

Frames can be transmitted at once using **sf_serialmac_tx_frame()**. Or by
starting a frame with **sf_serialmac_tx_frame_start()** and successively
appending the payload using **sf_serialmac_tx_frame_append()** until the frame
is filled.

Whenever the MAC completed the transmission of a frame the upper layer's
callback called that has been registered as **SF_SERIALMAC_TX_EVENT tx_event()**
on initialization.

Whenever the MAC processed a buffer with payload the upper layer's callback
is called which has been registered as **SF_SERIALMAC_TX_EVENT tx_buf_event** on
initialization. The upper layer must not touch the buffer memory passed with
**sf_serialmac_tx_frame()** or **sf_serialmac_tx_frame_append()** before this
callback has been called. Also all calls to **sf_serialmac_tx_frame_append()**
are ignored until the previously provided buffer has been processed.

## Protocol v1/v2

Due to backward compatibility reasons the serial mac library v3.0.0 introduces the possibility to set the MAC protocol version at runtime.

_________________________________

[Serial Interface MAC](README.md)