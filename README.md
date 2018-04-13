# Serial Interface MAC
```
     ___ _____ _   ___ _  _____ ___  ___  ___ ___
    / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
    \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
    |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
    embedded.connectivity.solutions.==============
```

# Introduction

## Purpose

The STACKFORCE Serial MAC provides framing for serial interfaces.

On TX the STACKFORCE Serial MAC takes over the task to wrap up data in frames
before sending them over the serial interface.
On RX the STACKFORCE Serial MAC listens for incoming frames, verifies their
CRC and provides the payload to the upper layer.

## Features

The STACKFORCE Serial MAC is written with cross-platform portability in mind.
It should be usable within operating systems as well as bare metal devices.

* All API functions are non-blocking.
* The MAC has no direct dependencies (besides standard C libs and
STACKFORCE utilities that are hardware/OS independent, e.g. CRC module).
* The MAC is usable with any HAL library that provides non-blocking
functions to read from and write to the serial interface and a function
which returns the number of bytes waiting on input.
* Buffer allocation and management is completely left to the upper layer.

# Contents

  1. [Serial MAC Protocol Version 1](SerialMACProtocolv1.md)
  2. [Serial MAC Protocol Version 2](SerialMACProtocolv2.md)
  3. [Build Instructions](Build_Instructions.md)
  4. [Usage](Usage.md)

________________________
