# Serial Interface MAC: Serial MAC Protocol V1
```
     ___ _____ _   ___ _  _____ ___  ___  ___ ___
    / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
    \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
    |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
    embedded.connectivity.solutions.==============
```

# Serial MAC Protocol V1

The Serial MAC Protocol version describes the structure of a Serial MAC frame.
Please note that this version is independent from the product version.

## Frame Format

    +--------------------+-----------------+-- - - --+--------------+
    | SYNC BYTE [1 Byte] | LENGTH [2 Byte] | PAYLOAD | CRC [2 Byte] |
    +--------------------+-----------------+-- - - --+--------------+

  - **SYNC BYTE:** Fixed value 0xA5.
  - **LENGTH:** Transmitted payload size. HEADER and CRC are not counted.
  - **PAYLOAD:** Payload of the Serial MAC frame. Variable length which is described through the length field. The content of the MAC PAYLOAD is the serial protocol.
  - **CRC:** Cyclic redundancy check sum over the MAC PAYLOAD. The CRC polynomial is:

         x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^2 + 1

## Example
* Payload = 0x01

      +----+-------+----+-------+
      | A5 | 00 01 | 01 | C2 9A |
      +----+-------+----+-------+

_________________________________

[Serial Interface MAC](README.md)