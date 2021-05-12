## Introduction

CloudLamp is an LED-based lamp that looks and sounds like a thundercloud (and other clouds sometimes).  This repository contains information about the design and relevant software.  If you are assembling one from pieces you received, see [here](setup.md).

![CloudLamp demo](assets/CloudLamp.gif)

## System overview

The CloudLamp consists of a base and a cloud.  The base contains most of the physical inputs and outputs (buttons, dials, pump, power supply input) while the cloud contains most of the logic and the LED driver (see [hardware](hardware.ino)).

## Repository contents

* [Firmware controlling the base](base)
* [Firmware controlling the cloud](cloud)
* [Diagnostic tool](cloud_serial_diagnostic) to communicate directly with the base Arduino Uno while connected to the cloud ESP32
* [Tool to generate the shape of the shade wireframe](CloudGen)

Also related is [this repository](https://github.com/BenjaminPelletier/ThunderGen) to generate the synthetic thunder sounds.

